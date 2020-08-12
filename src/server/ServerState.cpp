#include "ServerState.h"

#include "sf/HashSet.h"
#include "sf/Mutex.h"

#include "ext/json_input.h"
#include "sp/Json.h"
#include "sf/File.h"

#include "sf/Reflection.h"

#include "server/Pathfinding.h"

#include "server/FixedPoint.h"

#include <stdarg.h>

#include <random>

namespace sv {

Component *Prefab::findComponentImp(Component::Type type) const
{
	for (Component *component : components) {
		if (component->type == type) return component;
	}
	return nullptr;
}

sf_inline Prefab *findPrefabExisting(ServerState &state, const sf::Symbol &name)
{
	Prefab *prefab = state.prefabs.find(name);
	if (!prefab) {
		sf::debugPrintLine("Could not find prefab: %s", name.data);
	}
	return prefab;
}

sf_inline Prop *findProp(ServerState &state, uint32_t id)
{
	Prop *chr = state.props.find(id);
	if (!chr) {
		sf::debugPrintLine("Could not find prop: %u", id);
	}
	return chr;
}

sf_inline Character *findCharacter(ServerState &state, uint32_t id)
{
	Character *chr = state.characters.find(id);
	if (!chr) {
		sf::debugPrintLine("Could not find character: %u", id);
	}
	return chr;
}

sf_inline Card *findCard(ServerState &state, uint32_t id)
{
	Card *card = state.cards.find(id);
	if (!card) {
		sf::debugPrintLine("Could not find card: %u", id);
	}
	return card;
}

sf_inline Status *findStatus(ServerState &state, uint32_t id)
{
	Status *status = state.statuses.find(id);
	if (!status) {
		sf::debugPrintLine("Could not find status: %u", id);
	}
	return status;
}

template <typename T>
sf_inline T *findComponent(const Prefab &prefab)
{
	T *t = (T*)prefab.findComponentImp(T::ComponentType);
	if (!t) {
		sf::debugPrintLine("Could not find component %u from %s", (uint32_t)T::ComponentType, prefab.name.data);
	}
	return t;
}

sf_inline bool check(bool cond, const char *msg)
{
	if (!cond) {
		sf::debugPrintLine("Error: %s", msg);
	}
	return cond;
}

#define sv_check(cond) check((cond), #cond)

sf_inline void pushEvent(ServerState &state, sf::Array<sf::Box<Event>> &events, const sf::Box<Event> &event)
{
	state.applyEvent(*event);
	events.push(event);
}

static uint32_t rollDie(uint32_t max)
{
	// TODO: Proper random
	return rand() % (max - 1) + 1;
}

static RollInfo rollDice(const DiceRoll &roll, sf::Symbol name)
{
	RollInfo info;
	info.roll = roll;
	uint32_t total = roll.bias;
	for (uint32_t i = 0; i < roll.num; i++) {
		uint32_t value = rollDie(roll.die);
		total += value;
		info.results.push(value);
	}
	info.total = total;
	return info;
}

sf_inline RollInfo rollDice(const DiceRoll &roll, const char *name)
{
	return rollDice(roll, sf::Symbol(name));
}

ServerState::ServerState()
{
	// TODO: Figure something proper out for this
	prefabs.reserve(1024);
	props.reserve(1024);
	characters.reserve(1024);
	cards.reserve(1024);
	statuses.reserve(1024);
	charactersToSelect.reserve(1024);
}

uint32_t ServerState::allocateId(sf::Array<sf::Box<Event>> &events, IdType type, bool local)
{
	uint32_t nextId = local ? lastLocalAllocatedIdByType[(uint32_t)type] : lastAllocatedIdByType[(uint32_t)type];
	for (;;) {
		nextId++;
		if (nextId >= MaxIdIndex) nextId = 1;

		uint32_t id = makeId(type, local ? nextId + MaxServerIdIndex : nextId);

		bool exists = false;
		switch (type) {
		case IdType::Prop: exists = !!props.find(id); break;
		case IdType::Character: exists = !!characters.find(id); break;
		case IdType::Card: exists = !!cards.find(id); break;
		case IdType::Status: exists = !!statuses.find(id); break;
		default: sf_failf("Unhandled ID type %u", (uint32_t)type);
		}

		if (!exists) {
			auto e = sf::box<AllocateIdEvent>();
			e->id = id;
			pushEvent(*this, events, e);
			return id;
		}
	}
}

sf_inline bool isCharacterValid(const ServerState &state, uint32_t chrId)
{
	const Character *chr = state.characters.find(chrId);
	return chr != nullptr;;
}

sf_inline bool isPropValid(const ServerState &state, uint32_t propId)
{
	const Prop *prop = state.props.find(propId);
	return prop != nullptr;
}

sf_inline bool isCardValid(const ServerState &state, uint32_t cardId)
{
	const Card *card = state.cards.find(cardId);
	return card != nullptr;
}

sf_inline bool isStatusValid(const ServerState &state, uint32_t propId)
{
	const Status *status = state.statuses.find(propId);
	return status != nullptr;
}

bool ServerState::isIdValid(uint32_t id)
{
	switch (getIdType(id)) {

	case IdType::Prop: return isPropValid(*this, id);
	case IdType::Character: return isCharacterValid(*this, id);
	case IdType::Card: return isCardValid(*this, id);
	case IdType::Status: return isStatusValid(*this, id);
	default:
		sf_failf("Invalid ID type: %u", (uint32_t)getIdType(id));
		return false;
	}
}

static void addObbToTiles(ServerState &state, uint32_t id, const sf::Vec2i &min, const sf::Vec2i &max, const sf::Vec2i &position, uint32_t rotation)
{
	int32_t cos = sv::fixedCos360(rotation);
	int32_t sin = sv::fixedSin360(rotation);

	sf::Vec2i dx = sf::Vec2i(cos, -sin);
	sf::Vec2i dy = sf::Vec2i(sin, cos);

	int32_t xx0 = position.x + sv::fixedMul(dx.x, min.x);
	int32_t xx1 = position.x + sv::fixedMul(dx.x, max.x);
	int32_t yy0 = position.y + sv::fixedMul(dy.y, min.y);
	int32_t yy1 = position.y + sv::fixedMul(dy.y, max.y);

	int32_t xy0 = sv::fixedMul(dy.x, min.y);
	int32_t xy1 = sv::fixedMul(dy.x, max.y);
	int32_t yx0 = sv::fixedMul(dx.y, min.x);
	int32_t yx1 = sv::fixedMul(dx.y, max.x);

	int32_t minX = (sf::min(xx0, xx1) + sf::min(xy0, xy1) - 0x7fff) >> 16;
	int32_t minY = (sf::min(yy0, yy1) + sf::min(yx0, yx1) - 0x7fff) >> 16;
	int32_t maxX = (sf::max(xx0, xx1) + sf::max(xy0, xy1) + 0x7fff) >> 16;
	int32_t maxY = (sf::max(yy0, yy1) + sf::max(yx0, yx1) + 0x7fff) >> 16;

	sf::Vec2i topLeft = sf::Vec2i(minX << 16, minY << 16) - position;
	int32_t ppx = sv::fixedDot(topLeft, dx);
	int32_t ppy = sv::fixedDot(topLeft, dy);

	for (int32_t y = minY; y <= maxY; y++) {
		int32_t px = ppx, py = ppy;

		for (int32_t x = minX; x <= maxX; x++) {
			int32_t cx = sf::clamp(px, min.x, max.x) - px;
			int32_t cy = sf::clamp(py, min.y, max.y) - py;
			int32_t distSq = fixedMul(cx, cx) + fixedMul(cy, cy);

			if (distSq < 0x3840) {
				state.addEntityToTile(id, sf::Vec2i(x, y));
			}
			px += dx.x;
			py += dy.x;
		}

		ppx += dx.y;
		ppy += dy.y;
	}
}

static void addEntityToTiles(ServerState &state, uint32_t id, const sf::Symbol &prefabName, const sf::Vec2i &position, uint32_t rotation, int32_t scale)
{
	Prefab *prefab = findPrefabExisting(state, prefabName);
	if (!prefab) return;

	for (const Component *component : prefab->components) {
		if (const auto *c = component->as<TileAreaComponent>()) {
			sf::Vec2i min, max;
			min.x = sv::fixedMul(c->minCorner.x, scale);
			min.y = sv::fixedMul(c->minCorner.y, scale);
			max.x = sv::fixedMul(c->maxCorner.x, scale);
			max.y = sv::fixedMul(c->maxCorner.y, scale);
			addObbToTiles(state, id, min, max, position, rotation);
		}
	}
}

static void addPropToTiles(ServerState &state, const sv::Prop &prop)
{
	uint32_t rotation = prop.transform.rotation >> 6;
	addEntityToTiles(state, prop.id, prop.prefabName, prop.transform.position, rotation, prop.transform.scale << 8);
}

static void addCharacterToTiles(ServerState &state, const sv::Character &chr)
{
	state.addEntityToTile(chr.id, chr.tile);
}

void ServerState::applyEvent(const Event &event)
{
	// TODO: What to do about reallocations?
	if (auto *e = event.as<AllocateIdEvent>()) {
		uint32_t index = getIdIndex(e->id);
		if (index < MaxServerIdIndex) {
			lastAllocatedIdByType[(uint32_t)getIdType(e->id)] = index;
		} else {
			lastLocalAllocatedIdByType[(uint32_t)getIdType(e->id)] = index - MaxServerIdIndex;
		}
	} else if (auto *e = event.as<CardCooldownTickEvent>()) {
		if (Card *card = findCard(*this, e->cardId)) {
			if (sv_check(card->cooldownLeft > 0)) {
				card->cooldownLeft--;
			}
		}
	} else if (auto *e = event.as<StatusAddEvent>()) {
		auto res = statuses.insertOrAssign(e->status);
		sv_check(res.inserted);
		if (Character *chr = findCharacter(*this, e->status.characterId)) {
			chr->statuses.push(e->status.id);
		}
	} else if (auto *e = event.as<StatusTickEvent>()) {
		if (Status *status = findStatus(*this, e->statusId)) {
			if (sv_check(status->turnsLeft > 0)) {
				status->turnsLeft--;
			}
		}
	} else if (auto *e = event.as<StatusRemoveEvent>()) {
		if (Status *status = findStatus(*this, e->statusId)) {
			if (Character *chr = findCharacter(*this, status->characterId)) {
				sf::findRemoveSwap(chr->statuses, e->statusId);
			}
		}
	} else if (auto *e = event.as<DamageEvent>()) {
		if (Character *chr = findCharacter(*this, e->damageInfo.targetId)) {
			chr->health -= (int32_t)e->finalDamage;
		}
	} else if (auto *e = event.as<LoadPrefabEvent>()) {
		auto res = prefabs.insertOrAssign(e->prefab);
		sv_check(res.inserted);
	} else if (auto *e = event.as<ReloadPrefabEvent>()) {
		prefabs[e->prefab.name] = e->prefab;

		if (const sf::UintSet *propIds = prefabProps.findValue(e->prefab.name)) {
			for (uint32_t propId : *propIds) {
				if (const Prop *prop = props.find(propId)) {
					removeEntityFromAllTiles(propId);
					addPropToTiles(*this, *prop);
				}
			}
		}

	} else if (auto *e = event.as<RemoveGarbageIdsEvent>()) {
		removeIds(e->ids.slice());
	} else if (auto *e = event.as<RemoveGarbagePrefabsEvent>()) {
		removePrefabs(e->names.slice());
	} else if (auto *e = event.as<AddPropEvent>()) {
		sv_check(!isPropValid(*this, e->prop.id));
		props.insertOrAssign(e->prop);

		prefabProps[e->prop.prefabName].insertIfNew(e->prop.id);

		addPropToTiles(*this, e->prop);
	} else if (auto *e = event.as<RemovePropEvent>()) {
		if (Prop *prop = findProp(*this, e->propId)) {
			prefabProps[prop->prefabName].removeOne(e->propId);
		}

		bool removed = props.remove(e->propId);
		sv_check(removed);

		removeEntityFromAllTiles(e->propId);
	} else if (auto *e = event.as<ReplaceLocalPropEvent>()) {
		sv_check(!isPropValid(*this, e->prop.id));

		props.insertOrAssign(e->prop);
		if (localClientId == e->clientId) {
			if (Prop *prop = findProp(*this, e->localId)) {
				prefabProps[prop->prefabName].removeOne(e->localId);
				removeEntityFromAllTiles(e->localId);
				props.remove(e->localId);
			}
		}

		prefabProps[e->prop.prefabName].insertIfNew(e->prop.id);
		addPropToTiles(*this, e->prop);

	} else if (auto *e = event.as<AddCharacterEvent>()) {
		auto res = characters.insertOrAssign(e->character);
		sv_check(res.inserted);

		addCharacterToTiles(*this, e->character);

	} else if (auto *e = event.as<AddCardEvent>()) {
		auto res = cards.insertOrAssign(e->card);
		sv_check(res.inserted);
	} else if (auto *e = event.as<MovePropEvent>()) {
		if (Prop *prop = findProp(*this, e->propId)) {
			removeEntityFromAllTiles(e->propId);
			prop->transform = e->transform;
			addPropToTiles(*this, *prop);
		}

	} else if (auto *e = event.as<GiveCardEvent>()) {
		if (Card *card = findCard(*this, e->cardId)) {
			sv_check(card->ownerId == e->previousOwnerId);

			if (card->ownerId) {
				if (Character *chr = findCharacter(*this, card->ownerId)) {
					for (uint32_t &idRef : chr->selectedCards) {
						if (idRef == card->id) {
							idRef = 0; 
						}
					}
					sf::findRemoveSwap(chr->cards, card->id);
				}
			}

			if (e->ownerId) {
				if (Character *chr = findCharacter(*this, e->ownerId)) {
					chr->cards.push(e->cardId);
				}
			}

			card->ownerId = e->ownerId;
		}
	} else if (auto *e = event.as<SelectCardEvent>()) {
		if (Character *chr = findCharacter(*this, e->ownerId)) {
			bool found = sf::find(chr->cards, e->cardId);
			if (sv_check(found)) {
				chr->selectedCards[e->slot] = e->cardId;
			}
		}
	} else if (auto *e = event.as<AddCharacterToSpawn>()) {
		charactersToSelect[e->selectPrefab] += e->count;
	} else if (auto *e = event.as<SelectCharacterToSpawnEvent>()) {
		charactersToSelect[e->selectPrefab]--;
	}
}

void ServerState::getAsEvents(EventCallbackFn *callback, void *user) const
{
	for (const Prefab &prefab : prefabs) {
		LoadPrefabEvent e = { };
		e.prefab = prefab;
		callback(user, e);
	}

	for (const Prop &prop : props) {
		AddPropEvent e = { };
		e.prop = prop;
		callback(user, e);
	}

	for (const Character &chr : characters) {
		AddCharacterEvent e = { };
		e.character = chr;
		e.character.cards.clear();
		sf::memZero(e.character.selectedCards);
		callback(user, e);
	}

	for (const Card &card : cards) {
		AddCardEvent e = { };
		e.card = card;
		e.card.ownerId = 0;
		callback(user, e);
	}

	for (const auto &pair : charactersToSelect) {
		AddCharacterToSpawn e = { };
		e.selectPrefab = pair.key;
		e.count = pair.val;
		callback(user, e);
	}

	for (uint32_t i = 0; i < NumServerIdTypes; i++) {
		AllocateIdEvent e = { };
		e.id = makeId((IdType)i, lastAllocatedIdByType[i]);
		callback(user, e);
	}

	for (uint32_t i = 0; i < NumServerIdTypes; i++) {
		AllocateIdEvent e = { };
		e.id = makeId((IdType)i, lastLocalAllocatedIdByType[i]);
		callback(user, e);
	}

	for (const Character &chr : characters) {
		for (uint32_t cardId : chr.cards) {
			GiveCardEvent e = { };
			e.previousOwnerId = 0;
			e.ownerId = chr.id;
			e.cardId = cardId;
			callback(user, e);
		}

		for (uint32_t slot = 0; slot < NumSelectedCards; slot++) {
			uint32_t cardId = chr.selectedCards[slot];
			if (cardId == 0) continue;

			SelectCardEvent e = { };
			e.ownerId = chr.id;
			e.cardId = cardId;
			e.slot = slot;
			callback(user, e);
		}
	}

	for (const auto &pair : charactersToSelect) {
		AddCharacterToSpawn e = { };
		e.selectPrefab = pair.key;
		e.count = pair.val;
		callback(user, e);
	}

	for (const Status &status : statuses) {
		StatusAddEvent e = { };
		sf::memZero(e.turnsRoll);
		e.status = status;
		callback(user, e);
	}
}

static sf::StaticRecursiveMutex g_configMutex;

template <typename T>
static sf::Box<T> loadConfig(sf::String name)
{
	sf::Symbol path = sf::Symbol(name);

	sf::RecursiveMutexGuard mg(g_configMutex);

	static sf::HashMap<sf::Symbol, sf::Box<T>> cache;
	sf::Box<T> &entry = cache[path];
	if (entry) return entry;

	jsi_args args = { };
	args.dialect.allow_bare_keys = true;
	args.dialect.allow_comments = true;
	args.dialect.allow_control_in_string = true;
	args.dialect.allow_missing_comma = true;
	args.dialect.allow_trailing_comma = true;
	jsi_value *value = jsi_parse_file(path.data, &args);
	if (!value) {
		sf::debugPrint("Failed to parse %s:%u:%u: %s",
			path.data, args.error.line, args.error.column, args.error.description);
		return { };
	}

	if (!sp::readJson(value, entry)) return { };

	entry->name = path;

	return entry;
}

static void walkPrefabs(ServerState &state, sf::Array<sf::Box<Event>> *events, sf::HashSet<sf::Symbol> *marks, const sf::Symbol &name)
{
	if (!name) return;
	if (marks) {
		if (!marks->insert(name).inserted) return;
	}
	Prefab *prefab = state.prefabs.find(name);

	if (events) {
		if (prefab) return;

		sf::Box<Prefab> box = loadConfig<Prefab>(name);
		if (!box) return;
		
		{
			auto e = sf::box<LoadPrefabEvent>();
			e->prefab = *box;
			pushEvent(state, *events, e);
		}
		prefab = box;
	}

	for (Component *component : prefab->components) {
		if (auto *c = component->as<CardAttachComponent>()) {
			walkPrefabs(state, events, marks, c->prefabName);
		} else if (auto *c = component->as<CardProjectileComponent>()) {
			walkPrefabs(state, events, marks, c->prefabName);
		} else if (auto *c = component->as<CastOnReceiveDamageComponent>()) {
			walkPrefabs(state, events, marks, c->spellName);
		} else if (auto *c = component->as<CastOnDealDamageComponent>()) {
			walkPrefabs(state, events, marks, c->spellName);
		} else if (auto *c = component->as<CardCastComponent>()) {
			walkPrefabs(state, events, marks, c->spellName);
		} else if (auto *c = component->as<SpellStatusComponent>()) {
			walkPrefabs(state, events, marks, c->statusName);
		} else if (auto *c = component->as<StatusComponent>()) {
			walkPrefabs(state, events, marks, c->startEffect);
			walkPrefabs(state, events, marks, c->activeEffect);
			walkPrefabs(state, events, marks, c->endEffect);
		} else if (auto *c = component->as<CharacterTemplateComponent>()) {
			walkPrefabs(state, events, marks, c->characterPrefab);
			for (const sf::Symbol &cardPrefab : c->starterCardPrefabs) {
				walkPrefabs(state, events, marks, cardPrefab);
			}
		}
	}
}

sf_inline Prefab *loadPrefab(ServerState &state, sf::Array<sf::Box<Event>> &events, const sf::Symbol &name)
{
	if (Prefab *prefab = state.prefabs.find(name)) {
		return prefab;
	}

	walkPrefabs(state, &events, nullptr, name);

	return state.prefabs.find(name);
}

void ServerState::putStatus(sf::Array<sf::Box<Event>> &events, const StatusInfo &statusInfo)
{
	Prefab *statusPrefab = loadPrefab(*this, events, statusInfo.statusName);
	if (!statusPrefab) return;

	StatusComponent *statusComp = findComponent<StatusComponent>(*statusPrefab);
	if (!statusComp) return;

	uint32_t id = allocateId(events, IdType::Status, false);

	{
		auto e = sf::box<StatusAddEvent>();
		e->turnsRoll = rollDice(statusComp->turnsRoll, "turns");
		e->status.id = id;
		e->status.prefabName = statusInfo.statusName;
		e->status.cardName = statusInfo.cardName;
		e->status.originalCasterId = statusInfo.originalCasterId;
		e->status.casterId = statusInfo.casterId;
		e->status.characterId = statusInfo.targetId;
		e->status.turnsLeft = e->turnsRoll.total;
		pushEvent(*this, events, e);
	}
}

void ServerState::doDamage(sf::Array<sf::Box<Event>> &events, const DamageInfo &damageInfo)
{
	Character *causeChr = findCharacter(*this, damageInfo.causeId);
	Character *targetChr = findCharacter(*this, damageInfo.targetId);
	if (!causeChr || !targetChr) return;

	auto e = sf::box<DamageEvent>();
	e->damageInfo = damageInfo;
	e->damageRoll = rollDice(damageInfo.damageRoll, "damage");
	e->meleeArmor = targetChr->armor;

	if (damageInfo.melee) {
		uint32_t total = 0;
		for (uint32_t val : e->damageRoll.results) {
			// TODO: > ?
			uint32_t damage = e->damageRoll.total;
			if (val == e->damageRoll.roll.die) {
				// Crit doubles melee damage
				damage *= 2;
			}
			if (val < e->meleeArmor) {
				// Armor halves melee damage
				damage /= 2;
			}
			total += damage;
		}
		e->finalDamage = total;
	} else {
		e->finalDamage = e->damageRoll.total;
	}

	for (uint32_t statusId : causeChr->statuses) {
		Status *status = findStatus(*this, statusId);
		if (!status) continue;
		Prefab *statusPrefab = loadPrefab(*this, events, status->prefabName);
		if (!statusPrefab) continue;

		for (Component *statusComponent : statusPrefab->components) {
			if (auto *c = statusComponent->as<ResistDamageComponent>()) {
				if ((c->onSpell && damageInfo.magic) || (c->onMelee && damageInfo.melee)) {
					RollInfo roll = rollDice(c->successRoll, "success");

					bool success = roll.total >= roll.roll.check;

					int32_t resistDamage = 0;
					if (success) {
						resistDamage = (uint32_t)sf::clamp((int32_t)(c->resistAmount * e->finalDamage), 0, (int32_t)e->finalDamage);
						e->finalDamage -= resistDamage;
					}

					auto e2 = sf::box<ResistDamageEvent>();
					e2->cardName = status->cardName;
					e2->effectName = c->effectName;
					e2->successRoll = roll;
					e2->resistAmount = c->resistAmount;
					e2->resistDamage = resistDamage;
					e2->success = success;
					pushEvent(*this, events, e2);
				}
			}
		}
	}

	pushEvent(*this, events, e);

	for (uint32_t statusId : causeChr->statuses) {
		Status *status = findStatus(*this, statusId);
		if (!status) continue;
		Prefab *statusPrefab = loadPrefab(*this, events, status->prefabName);
		if (!statusPrefab) continue;

		for (Component *statusComponent : statusPrefab->components) {
			if (auto *c = statusComponent->as<CastOnDealDamageComponent>()) {
				if ((c->onSpell && damageInfo.magic) || (c->onMelee && damageInfo.melee)) {
					SpellInfo spell = { };
					spell.spellName = c->spellName;
					spell.cardName = status->cardName;
					spell.originalCasterId = status->originalCasterId;
					spell.casterId = damageInfo.causeId;
					spell.targetId = damageInfo.targetId;
					castSpell(events, spell);
				}
			} else if (auto *c = statusComponent->as<ResistDamageComponent>()) {
				if ((c->onSpell && damageInfo.magic) || (c->onMelee && damageInfo.melee)) {
					auto e2 = sf::box<ResistDamageEvent>();
				}
			}
		}
	}


	for (uint32_t statusId : targetChr->statuses) {
		Status *status = findStatus(*this, statusId);
		if (!status) continue;
		Prefab *statusPrefab = loadPrefab(*this, events, status->prefabName);
		if (!statusPrefab) continue;

		for (Component *statusComponent : statusPrefab->components) {
			if (auto *c = statusComponent->as<CastOnReceiveDamageComponent>()) {
				if ((c->onSpell && damageInfo.magic) || (c->onMelee && damageInfo.melee)) {
					SpellInfo spell = { };
					spell.spellName = c->spellName;
					spell.cardName = status->cardName;
					spell.originalCasterId = status->originalCasterId;
					spell.casterId = damageInfo.targetId;
					spell.targetId = damageInfo.causeId;
					castSpell(events, spell);
				}
			}
		}
	}
}

void ServerState::castSpell(sf::Array<sf::Box<Event>> &events, const SpellInfo &spellInfo)
{
	Prefab *spellPrefab = loadPrefab(*this, events, spellInfo.spellName);
	if (!spellPrefab) return;

	SpellComponent *spellComp = findComponent<SpellComponent>(*spellPrefab);
	if (!spellComp) return;

	RollInfo successRoll = rollDice(spellComp->successRoll, "success");

	{
		auto e = sf::box<CastSpellEvent>();
		e->spellInfo = spellInfo;
		e->successRoll = successRoll;
		pushEvent(*this, events, e);
	}

	if (successRoll.total < successRoll.roll.check) return;

	for (Component *component : spellPrefab->components) {

		if (auto *c = component->as<SpellDamageComponent>()) {
			RollInfo damageRoll = rollDice(c->damageRoll, "damage");
			DamageInfo damage = { };
			damage.magic = true;
			damage.spellName = spellInfo.spellName;
			damage.originalCasterId = spellInfo.originalCasterId;
			damage.causeId = spellInfo.casterId;
			damage.targetId = spellInfo.targetId;
			damage.damageRoll = c->damageRoll;
			doDamage(events, damage);
		} else if (auto *c = component->as<SpellStatusComponent>()) {
			StatusInfo status = { };
			status.statusName = c->statusName;
			status.cardName = spellInfo.cardName;
			status.originalCasterId = spellInfo.originalCasterId;
			status.casterId = spellInfo.casterId;
			status.targetId = spellInfo.targetId;
			putStatus(events, status);
		}
	}
}

void ServerState::meleeAttack(sf::Array<sf::Box<Event>> &events, const MeleeInfo &meleeInfo)
{
	Character *attackerChr = findCharacter(*this, meleeInfo.attackerId);
	if (!attackerChr) return;

	if (!attackerChr->selectedCards[0]) return;
	Card *card = findCard(*this, attackerChr->selectedCards[0]);
	if (!card) return;

	Prefab *cardPrefab = loadPrefab(*this, events, card->prefabName);
	if (!cardPrefab) return;

	CardMeleeComponent *meleeComponent = findComponent<CardMeleeComponent>(*cardPrefab);
	if (!meleeComponent) return;

	{
		auto e = sf::box<MeleeAttackEvent>();
		e->meleeInfo = meleeInfo;
		e->hitRoll = meleeComponent->hitRoll;
		pushEvent(*this, events, e);
	}

	DamageInfo damage = { };
	damage.melee = true;
	damage.physical = true;
	damage.originalCasterId = meleeInfo.attackerId;
	damage.causeId = meleeInfo.attackerId;
	damage.targetId = meleeInfo.targetId;
	damage.damageRoll = meleeComponent->hitRoll;
	doDamage(events, damage);
}

void ServerState::startCharacterTurn(sf::Array<sf::Box<Event>> &events, uint32_t characterId)
{
	Character *chr = findCharacter(*this, characterId);
	if (!chr) return;

	for (uint32_t cardId : chr->cards) {
		Card *card = findCard(*this, cardId);
		if (!card) continue;

		if (card->cooldownLeft > 0) {
			auto e = sf::box<CardCooldownTickEvent>();
			e->cardId = cardId;
			pushEvent(*this, events, std::move(e));
		}
	}

	sf::SmallArray<uint32_t, 64> statusIds;
	statusIds.push(chr->statuses.slice());
	for (uint32_t id : statusIds) {
		Status *status = statuses.find(id);
		if (!status) continue;

		if (Prefab *statusPrefab = loadPrefab(*this, events, status->prefabName)) {
			for (Component *component : statusPrefab->components) {
				if (auto *c = component->as<CastOnTurnStartComponent>()) {
					SpellInfo spell = { };
					spell.originalCasterId = status->originalCasterId;
					spell.casterId = status->casterId;
					spell.targetId = status->characterId;
					spell.cardName = status->cardName;
					spell.spellName = c->spellName;
					castSpell(events, spell);
				}
			}
		}

		if (status->turnsLeft > 0) {
			auto e = sf::box<StatusTickEvent>();
			e->statusId = status->id;
			pushEvent(*this, events, std::move(e));
		} else {
			auto e = sf::box<StatusRemoveEvent>();
			e->statusId = status->id;
			pushEvent(*this, events, std::move(e));
		}
	}
}

void ServerState::preloadPrefab(sf::Array<sf::Box<Event>> &events, const sf::Symbol &name)
{
	loadPrefab(*this, events, name);
}

void ServerState::reloadPrefab(sf::Array<sf::Box<Event>> &events, const Prefab &prefab)
{
	// If the prefab doesn't exist just load it
	Prefab *existing = prefabs.find(prefab.name);
	if (!existing) {
		{
			auto e = sf::box<LoadPrefabEvent>();
			e->prefab = prefab;
			pushEvent(*this, events, e);
		}
		loadPrefab(*this, events, prefab.name);
		return;
	}

	// Reload prefab + load potential dependencies
	{
		auto e = sf::box<ReloadPrefabEvent>();
		e->prefab = prefab;
		pushEvent(*this, events, e);
	}
	loadPrefab(*this, events, prefab.name);
}

static void updateSpawnPosition(const ServerState &state, sf::Vec2i &bestTile, int32_t &bestDist, const sf::Vec2i &origin, int32_t dx, int32_t dy)
{
	sf::Vec2i delta = sf::Vec2i(dx, dy);
	int32_t dist = sf::dot(delta, delta);
	if (dist >= bestDist) return;

	sf::Vec2i tile = origin + delta;
	if (!isBlockedByPropOrCharacter(NULL, state, tile)) {
		bestTile = tile;
		bestDist = dist;
	}
}

static sf::Vec2i findSpawnableTile(const ServerState &state, const sf::Vec2i &origin)
{
	int32_t bestDist = INT32_MAX;
	sf::Vec2i bestTile = origin;

	for (int32_t radius = 0; radius < 10; radius++) {
		if (radius*radius > bestDist) break;

		for (int32_t y = -radius + 1; y <= radius - 1; y++) {
			updateSpawnPosition(state, bestTile, bestDist, origin, -radius, y);
			updateSpawnPosition(state, bestTile, bestDist, origin, +radius, y);
		}

		for (int32_t x = -radius; x <= radius; x++) {
			updateSpawnPosition(state, bestTile, bestDist, origin, x, -radius);
			updateSpawnPosition(state, bestTile, bestDist, origin, x, +radius);
		}
	}

	return bestTile;
}

uint32_t ServerState::selectCharacterSpawn(sf::Array<sf::Box<Event>> &events, const sf::Symbol &type, uint32_t playerId)
{
	int32_t *left = charactersToSelect.findValue(type);
	if (!left || *left == 0) return 0;

	Prefab *selectPrefab = loadPrefab(*this, events, type);
	if (!selectPrefab) return 0;

	CharacterTemplateComponent *templateComp = findComponent<CharacterTemplateComponent>(*selectPrefab);
	if (!templateComp) return 0;

	{
		auto e = sf::box<SelectCharacterToSpawnEvent>();
		e->selectPrefab = type;
		e->playerId = playerId;
		pushEvent(*this, events, e);
	}

	Character chrProto = { };
	chrProto.prefabName = templateComp->characterPrefab;
	chrProto.tile = findSpawnableTile(*this, sf::Vec2i(0, 0));

	uint32_t chrId = addCharacter(events, chrProto);
	if (!chrId) return 0 ;

	for (const sf::Symbol &cardPrefab : templateComp->starterCardPrefabs) {
		Card cardProto = { };
		cardProto.prefabName = cardPrefab;
		uint32_t cardId = addCard(events, cardProto);
		if (!cardId) return 0;

		giveCard(events, cardId, chrId);
	}

	return chrId;
}

uint32_t ServerState::addProp(sf::Array<sf::Box<Event>> &events, const Prop &prop, bool local)
{
	Prefab *prefab = loadPrefab(*this, events, prop.prefabName);
	if (!prefab) return 0;

	uint32_t id = allocateId(events, IdType::Prop, local);

	{
		auto e = sf::box<AddPropEvent>();
		e->prop = prop;
		e->prop.id = id;
		pushEvent(*this, events, e);
	}

	return id;
}

void ServerState::removeProp(sf::Array<sf::Box<Event>> &events, uint32_t propId)
{
	{
		auto e = sf::box<RemovePropEvent>();
		e->propId = propId;
		pushEvent(*this, events, e);
	}
}

uint32_t ServerState::replaceLocalProp(sf::Array<sf::Box<Event>> &events, const Prop &prop, uint32_t clientId, uint32_t localId)
{
	Prefab *prefab = loadPrefab(*this, events, prop.prefabName);
	if (!prefab) return 0;

	uint32_t id = allocateId(events, IdType::Prop, false);

	{
		auto e = sf::box<ReplaceLocalPropEvent>();
		e->clientId = clientId;
		e->localId = localId;
		e->prop = prop;
		e->prop.id = id;
		pushEvent(*this, events, e);
	}

	return id;
}

uint32_t ServerState::addCharacter(sf::Array<sf::Box<Event>> &events, const Character &chr, bool local)
{
	Prefab *prefab = loadPrefab(*this, events, chr.prefabName);
	if (!prefab) return 0;

	CharacterComponent *chrComp = findComponent<CharacterComponent>(*prefab);
	if (!chrComp) return 0;

	uint32_t id = allocateId(events, IdType::Character, local);

	{
		auto e = sf::box<AddCharacterEvent>();
		e->character = chr;
		e->character.health = e->character.maxHealth = chrComp->maxHealth;
		e->character.armor = chrComp->baseArmor;
		e->character.id = id;
		pushEvent(*this, events, e);
	}

	return id;
}

uint32_t ServerState::addCard(sf::Array<sf::Box<Event>> &events, const Card &card, bool local)
{
	Prefab *prefab = loadPrefab(*this, events, card.prefabName);
	if (!prefab) return 0;

	uint32_t id = allocateId(events, IdType::Card, local);

	{
		auto e = sf::box<AddCardEvent>();
		e->card = card;
		e->card.ownerId = 0;
		e->card.id = id;
		pushEvent(*this, events, e);
	}

	return id;
}

void ServerState::addCharacterToSelect(sf::Array<sf::Box<Event>> &events, const sf::Symbol &type, int32_t count)
{
	Prefab *selectPrefab = loadPrefab(*this, events, type);
	if (!selectPrefab) return;

	CharacterTemplateComponent *templateComp = findComponent<CharacterTemplateComponent>(*selectPrefab);
	if (!templateComp) return;

	{
		auto e = sf::box<AddCharacterToSpawn>();
		e->selectPrefab = type;
		e->count = count;
		pushEvent(*this, events, e);
	}
}

void ServerState::moveProp(sf::Array<sf::Box<Event>> &events, uint32_t propId, const PropTransform &transform)
{
	{
		auto e = sf::box<MovePropEvent>();
		e->propId = propId;
		e->transform = transform;
		pushEvent(*this, events, e);
	}
}

void ServerState::giveCard(sf::Array<sf::Box<Event>> &events, uint32_t cardId, uint32_t ownerId)
{
	Card *card = findCard(*this, cardId);
	if (!card) return;

	if (card->ownerId == ownerId) return;

	{
		auto e = sf::box<GiveCardEvent>();
		e->cardId = cardId;
		e->previousOwnerId = card->ownerId;
		e->ownerId = ownerId;
		pushEvent(*this, events, e);
	}

	if (ownerId) {
		Character *chr = findCharacter(*this, ownerId);
		if (!chr) return;

		Prefab *chrPrefab = loadPrefab(*this, events, chr->prefabName);
		if (!chrPrefab) return;

		CharacterComponent *chrComp = findComponent<CharacterComponent>(*chrPrefab);
		if (!chrComp) return;

		Prefab *cardPrefab = loadPrefab(*this, events, card->prefabName);
		if (!cardPrefab) return;

		CardComponent *cardComp = findComponent<CardComponent>(*cardPrefab);
		if (!cardComp) return;

		uint32_t lastMeleeSlot = 1;
		uint32_t lastSkillSlot = lastMeleeSlot + chrComp->skillSlots;
		uint32_t lastSpellSlot = lastSkillSlot + chrComp->spellSlots;
		uint32_t lastItemSlot = lastSpellSlot + chrComp->itemSlots;

		for (uint32_t i = 0; i < NumSelectedCards; i++) {
			if (chr->selectedCards[i] != 0) continue;

			bool select = false;
			if (i < lastMeleeSlot) {
				select = cardComp->melee;
			} else if (i < lastSkillSlot) {
				select = cardComp->skill;
			} else if (i < lastSpellSlot) {
				select = cardComp->spell;
			} else if (i < lastItemSlot) {
				select = cardComp->item;
			}
			if (select) {
				selectCard(events, cardId, ownerId, i);
			}
		}
	}
}

void ServerState::selectCard(sf::Array<sf::Box<Event>> &events, uint32_t cardId, uint32_t ownerId, uint32_t slot)
{
	{
		auto e = sf::box<SelectCardEvent>();
		e->cardId = cardId;
		e->ownerId = ownerId;
		e->slot = slot;
		pushEvent(*this, events, e);
	}
}

static void markId(const ServerState &state, sf::HashSet<uint32_t> &marks, uint32_t id)
{
	if (marks.insert(id).inserted) return;

	switch (getIdType(id)) {

	case IdType::Prop:
		break;

	case IdType::Character:
		if (const Character *chr = state.characters.find(id)) {
			for (uint32_t id : chr->cards) {
				markId(state, marks, id);
			}
			for (uint32_t id : chr->statuses) {
				markId(state, marks, id);
			}
		}
		break;

	case IdType::Card:
		break;

	case IdType::Status:
		break;

	default:
		sf_failf("Invalid ID type: %u", (uint32_t)getIdType(id));
		break;

	}
}

void ServerState::garbageCollectIds(sf::Array<uint32_t> &garbageIds) const
{
	sf::HashSet<uint32_t> marks;

	for (const Prop &prop : props) {
		markId(*this, marks, prop.id);
	}

	for (const Character &chr : characters) {
		markId(*this, marks, chr.id);
	}

	for (const Card &card : cards) {
		markId(*this, marks, card.id);
	}

	for (const Status &status : statuses) {
		markId(*this, marks, status.id);
	}

	for (const Prop &prop : props) {
		if (!marks.find(prop.id)) garbageIds.push(prop.id);
	}

	for (const Character &chr : characters) {
		if (!marks.find(chr.id)) garbageIds.push(chr.id);
	}

	for (const Card &card : cards) {
		if (!marks.find(card.id)) garbageIds.push(card.id);
	}

	for (const Status &status : statuses) {
		if (!marks.find(status.id)) garbageIds.push(status.id);
	}
}

static void markPrefab(const ServerState &state, sf::HashSet<sf::Symbol> &marks, const sf::Symbol &prefabName)
{
	walkPrefabs((ServerState&)state, nullptr, &marks, prefabName);
}

void ServerState::garbageCollectPrefabs(sf::Array<sf::Symbol> &garbagePrefabs) const
{
	sf::HashSet<sf::Symbol> marks;
	for (const Prop &prop : props) {
		markPrefab(*this, marks, prop.prefabName);
	}
	for (const Character &chr : characters) {
		markPrefab(*this, marks, chr.prefabName);
	}
	for (const Card &card : cards) {
		markPrefab(*this, marks, card.prefabName);
	}
	for (const auto &pair : charactersToSelect) {
		markPrefab(*this, marks, pair.key);
	}

	if (marks.size() < prefabs.size()) {
		garbagePrefabs.reserve(prefabs.size() - marks.size());
	}
	for (const Prefab &prefab : prefabs) {
		if (marks.find(prefab.name)) continue;
		garbagePrefabs.push(prefab.name);
	}
}

void ServerState::removeIds(sf::Slice<const uint32_t> ids)
{
	for (uint32_t id : ids) {
		switch (getIdType(id)) {
		case IdType::Prop: props.remove(id); break;
		case IdType::Character: characters.remove(id); break;
		case IdType::Card: cards.remove(id); break;
		case IdType::Status: statuses.remove(id); break;
		}
	}
}

void ServerState::removePrefabs(sf::Slice<const sf::Symbol> names)
{
	for (const sf::Symbol &name : names) {
		prefabs.remove(name);
	}
}

static sf::Symbol makeUniquePrefabName(const sf::Symbol &parentName)
{
	sf::SmallStringBuf<128> name;
	name.append('<');

	if (sf::beginsWith(parentName, "<")) {
		size_t colon = sf::indexOf(parentName, ':');
		if (colon != SIZE_MAX) {
			name.append(sf::String(parentName).substring(1, colon - 1));
		}
	} else {
		uint32_t begin = (uint32_t)parentName.size();
		uint32_t end = begin;
		while (begin > 0 && parentName.data[begin - 1] != '/') {
			begin--;
			if (parentName.data[begin] == '.') {
				end = begin;
			}
		}

		name.append(sf::String(parentName).substring(begin, end - begin));
	}

	{
		uint32_t uuid[4];
		std::random_device dev;
		for (uint32_t &p : uuid) {
			p = (uint32_t)dev();
		}
		name.format(":%08x-%08x-%08x-%08x>", uuid[0], uuid[1], uuid[2], uuid[3]);
	}

	return sf::Symbol(name);
}


void ServerState::applyEdit(sf::Array<sf::Box<Event>> &events, const Edit &edit, sf::Array<sf::Box<Edit>> &undoBuf)
{
	if (const auto *ed = edit.as<PreloadPrefabEdit>()) {

		preloadPrefab(events, ed->prefabName);

	} else if (const auto *ed = edit.as<ModifyPrefabEdit>()) {

		if (Prefab *prefab = prefabs.find(ed->prefab.name)) {
			auto ud = sf::box<ModifyPrefabEdit>();
			ud->prefab = *prefab;
			undoBuf.push(ud);
		}

		reloadPrefab(events, ed->prefab);

	} else if (const auto *ed = edit.as<MakeUniquePrefabEdit>()) {

		sf::Symbol cloneName = makeUniquePrefabName(ed->prefabName);

		if (Prefab *prefab = prefabs.find(ed->prefabName)) {
			sv::Prefab clonePrefab;
			sf::SmallArray<char, 4096> cloneBuf;
			sf::writeBinary(cloneBuf, *prefab);
			sf::readBinary(cloneBuf.slice(), clonePrefab);

			clonePrefab.name = cloneName;
			reloadPrefab(events, clonePrefab);
		}

		sf::Array<uint32_t> propIds;
		propIds.reserve(ed->propIds.size);

		for (uint32_t propId : ed->propIds) {
			if (Prop *prop = props.find(propId)) {
				Prop clone = *prop;
				clone.prefabName = cloneName;

				auto ud2 = sf::box<AddPropEdit>();
				ud2->prop = *prop;
				undoBuf.push(ud2);

				uint32_t cloneId = addProp(events, clone);
				removeProp(events, clone.id);

				auto ud1 = sf::box<RemovePropEdit>();
				ud1->propId = cloneId;
				undoBuf.push(ud1);

				propIds.push(cloneId);
			}
		}

		{
			auto e = sf::box<MakeUniquePrefabEvent>();
			e->clientId = ed->clientId;
			e->prefabName = ed->prefabName;
			e->uniquePrefabName = cloneName;
			e->propIds = std::move(propIds);
			pushEvent(*this, events, e);
		}

	} else if (const auto *ed = edit.as<AddPropEdit>()) {

		uint32_t propId = addProp(events, ed->prop);

		{
			auto ud = sf::box<RemovePropEdit>();
			ud->propId = propId;
			undoBuf.push(ud);
		}

	} else if (const auto *ed = edit.as<ClonePropEdit>()) {

		uint32_t propId = replaceLocalProp(events, ed->prop, ed->clientId, ed->localId);

		{
			auto ud = sf::box<RemovePropEdit>();
			ud->propId = propId;
			undoBuf.push(ud);
		}

	} else if (const auto *ed = edit.as<MovePropEdit>()) {
		if (Prop *prop = props.find(ed->propId)) {

			{
				auto ud = sf::box<MovePropEdit>();
				ud->propId = ed->propId;
				ud->transform = prop->transform;
				undoBuf.push(ud);
			}

			moveProp(events, ed->propId, ed->transform);
		}
	} else if (const auto *ed = edit.as<RemovePropEdit>()) {
		if (Prop *prop = props.find(ed->propId)) {

			{
				auto ud = sf::box<AddPropEdit>();
				ud->prop = *prop;
				undoBuf.push(ud);
			}

			removeProp(events, ed->propId);
		}
	} else {
		sf_failf("Unhandled edit type: %u", edit.type);
	}
}

void ServerState::addEntityToTile(uint32_t id, const sf::Vec2i &tile)
{
	uint32_t key = packTile(tile);
	tileToEntity.insertIfNew(key, id);
	entityToTile.insertIfNew(id, key);
}

void ServerState::removeEntityFromTile(uint32_t id, const sf::Vec2i &tile)
{
	uint32_t key = packTile(tile);
	tileToEntity.removePotentialPair(key, id);
	entityToTile.removePotentialPair(id, key);
}

void ServerState::removeEntityFromAllTiles(uint32_t id)
{
	uint32_t key;
	sf::UintFind find = entityToTile.findAll(id);
	while (find.next(key)) {
		tileToEntity.removeExistingPair(key, id);
		entityToTile.removeFound(find);
	}
}

sf::UintFind ServerState::getTileEntities(const sf::Vec2i &tile) const
{
	uint32_t key = packTile(tile);
	return tileToEntity.findAll(key);
}

sf::UintFind ServerState::getEntityPackedTiles(uint32_t id) const
{
	return entityToTile.findAll(id);
}

}


namespace sf {
using namespace sv;

template<> void initType<ServerState>(Type *t)
{
	static Field fields[] = {
		sf_field(ServerState, prefabs),
		sf_field(ServerState, props),
		sf_field(ServerState, characters),
		sf_field(ServerState, cards),
		sf_field(ServerState, statuses),
		sf_field(ServerState, charactersToSelect),
		sf_field(ServerState, prefabProps),
		sf_field(ServerState, lastAllocatedIdByType),
		sf_field(ServerState, tileToEntity),
		sf_field(ServerState, entityToTile),
	};
	sf_struct(t, ServerState, fields);
}

}