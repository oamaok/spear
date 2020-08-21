#pragma once

#include "server/ServerState.h"

#include "client/System.h"

namespace sp { struct Canvas; }

namespace cl {

struct GameSystem : EntitySystem
{
	static sf::Box<GameSystem> create(const SystemsDesc &desc);

	virtual void writePersist(Systems &systems, ClientPersist &persist) = 0;

	virtual void updateCamera(FrameArgs &frameArgs) = 0;

	virtual void update(const sv::ServerState &svState, const FrameArgs &frameArgs) = 0;
	virtual void applyEvent(Systems &systems, const sv::Event &event) = 0;


	virtual void handleGui(Systems &systems, const GuiArgs &guiArgs) = 0;

};

}
