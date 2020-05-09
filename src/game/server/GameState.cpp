#include "GameState.h"

#include "sf/Reflection.h"
#include "game/server/Event.h"

namespace sv {

sf_inline void resolveChunk(sf::Vec2i &chunkI, sf::Vec2i &tileI, const sf::Vec2i &pos)
{
	chunkI = { pos.x >> MapChunk::SizeLog2, pos.y >> MapChunk::SizeLog2 };
	tileI = { pos.x & (MapChunk::Size - 1), pos.y & (MapChunk::Size - 1) };
}

void Map::setTile(const sf::Vec2i &pos, TileId tileId)
{
	sf::Vec2i chunkI, tileI;
	resolveChunk(chunkI, tileI, pos);
	auto result = chunks.insert(chunkI);
	MapChunk &chunk = result.entry.val;
	TileId &dst = chunks[chunkI].tiles[tileI.y * MapChunk::Size + tileI.x];
	if (dst == 0 && tileId != 0) chunk.numNonZeroTiles++;
	if (dst != 0 && tileId == 0) chunk.numNonZeroTiles--;
	dst = tileId;
	if (chunk.numNonZeroTiles == 0) {
		chunks.removeAt(&result.entry);
	}
}

TileId Map::getTile(const sf::Vec2i &pos) const
{
	sf::Vec2i chunkI, tileI;
	resolveChunk(chunkI, tileI, pos);
	auto it = chunks.find(chunkI);
	if (!it) return 0;
	return it->val.tiles[tileI.y * MapChunk::Size + tileI.x];
}

void EntityTileMap::impGrow(size_t minSize)
{
	size_t count, allocSize;
	rhmap_grow(&map, &count, &allocSize, minSize, 0);
	rhmap_rehash(&map, count, allocSize, sf::memAlloc(allocSize));
}

EntityTileMap::EntityTileMap()
{
	rhmap_init(&map);
}

EntityTileMap::EntityTileMap(const EntityTileMap &rhs)
{
	rhmap_init(&map);
	if (rhs.map.size > 0) impGrow(rhs.map.size);
	
	uint32_t hash, scan, value;
	while (rhmap_next(&rhs.map, &hash, &scan, &value)) {
		rhmap_insert(&map, hash, scan, value);
	}
}

EntityTileMap::EntityTileMap(EntityTileMap &&rhs)
{
	map = rhs.map;
	rhmap_reset(&rhs.map);
}

EntityTileMap &EntityTileMap::operator=(const EntityTileMap &rhs)
{
	if (&rhs == this) return *this;
	sf::memFree(rhmap_reset(&map));
	if (rhs.map.size > 0) impGrow(rhs.map.size);
	
	uint32_t hash, scan, value;
	while (rhmap_next(&rhs.map, &hash, &scan, &value)) {
		rhmap_insert(&map, hash, scan, value);
	}
	return *this;
}

EntityTileMap &EntityTileMap::operator=(EntityTileMap &&rhs)
{
	if (&rhs == this) return *this;
	map = rhs.map;
	rhmap_reset(&rhs.map);
	return *this;
}

EntityTileMap::~EntityTileMap()
{
	sf::memFree(rhmap_reset(&map));
}

void EntityTileMap::add(EntityId entity, const sf::Vec2i &newPos)
{
	if (map.size == map.capacity) impGrow(16);
	uint32_t newHash = sf::hash(newPos);
	rhmap_insert(&map, newHash, 0, entity);
}

void EntityTileMap::update(EntityId entity, const sf::Vec2i &oldPos, const sf::Vec2i &newPos)
{
	uint32_t oldHash = sf::hash(oldPos);
	uint32_t newHash = sf::hash(newPos);
	uint32_t scan = 0;
	rhmap_find_value(&map, oldHash, &scan, entity);
	rhmap_remove(&map, oldHash, scan);
	rhmap_insert(&map, newHash, 0, entity);
}

void EntityTileMap::remove(EntityId entity, const sf::Vec2i &oldPos)
{
	uint32_t oldHash = sf::hash(oldPos);
	uint32_t scan = 0;
	rhmap_find_value(&map, oldHash, &scan, entity);
	rhmap_remove(&map, oldHash, scan);
}

void EntityTileMap::reserve(size_t size)
{
	if (map.size + size > map.capacity) impGrow(size);
}

void EntityTileMap::clear()
{
	rhmap_clear(&map);
}

void State::refreshEntityTileMap()
{
	entityTileMap.clear();
	entityTileMap.reserve(entities.size);

	EntityId id = 0;
	for (auto &entity : entities) {
		if (entity) {
			entityTileMap.add(id, entity->position);
		}
		id++;
	}
}

void State::initEntity(EntityId entity, sf::Box<Entity> data)
{
	while (entity >= entities.size) entities.push();
	sf_assert(!entities[entity]);
	entityTileMap.add(entity, data->position);
	entities[entity] = std::move(data);
}

void State::destroyEntity(EntityId entity)
{
	Entity *data = entities[entity];
	uint32_t oldHash = sf::hash(data->position);
	entityTileMap.remove(entity, data->position);
	entities[entity].reset();
}

void State::setEntityPosition(EntityId entity, const sf::Vec2i &pos)
{
	Entity *data = entities[entity];
	if (pos == data->position) return;
	entityTileMap.update(entity, data->position, pos);
	data->position = pos;
}

void State::applyEvent(Event *event)
{
	if (auto e = event->as<EventMove>()) {
		setEntityPosition(e->entity, e->position);
	} else if (auto e = event->as<EventSpawn>()) {
		initEntity(e->entity, e->data);
	} else if (auto e = event->as<EventDestroy>()) {
		destroyEntity(e->entity);
	}
}

}

namespace sf {

template<> void initType<sv::TileType>(Type *t)
{
	static Field fields[] = {
		sf_field(sv::TileType, name),
		sf_field(sv::TileType, floor),
		sf_field(sv::TileType, wall),
	};
	sf_struct(t, sv::TileType, fields);
}

template<> void initType<sv::MapChunk>(Type *t)
{
	static Field fields[] = {
		sf_field_flags(sv::MapChunk, tiles, Field::CompactString),
		sf_field(sv::MapChunk, numNonZeroTiles),
	};
	sf_struct(t, sv::MapChunk, fields, Type::IsPod);
}

template<> void initType<sv::Entity::Type>(Type *t)
{
	static EnumValue values[] = {
		sf_enum(sv::Entity, None),
		sf_enum(sv::Entity, Character),
	};
	sf_enum_type(t, sv::Entity::Type, values);
}

template<> void initType<sv::Entity>(Type *t)
{
	static Field fields[] = {
		sf_field(sv::Entity, position),
	};
	static PolymorphType polys[] = {
		sf_poly(sv::Entity, Character, sv::Character),
	};
	sf_struct_poly(t, sv::Entity, type, fields, polys);
}

template<> void initType<sv::Character>(Type *t)
{
	static Field fields[] = {
		sf_field(sv::Character, name),
	};
	sf_struct_base(t, sv::Character, sv::Entity, fields);
}

template<> void initType<sv::Map>(Type *t)
{
	static Field fields[] = {
		sf_field(sv::Map, tileTypes),
		sf_field(sv::Map, chunks),
	};
	sf_struct(t, sv::Map, fields);
}

template<> void initType<sv::State>(Type *t)
{
	static Field fields[] = {
		sf_field(sv::State, map),
		sf_field(sv::State, entities),
	};
	sf_struct(t, sv::State, fields);
}


}