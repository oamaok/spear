#pragma once

#include "server/ServerState.h"

#include "client/System.h"

#include "sp/Sound.h"

namespace sf { struct FastRay; }

namespace cl {

struct AudioInfo
{
	sf::Vec3 position;
	float volume = 1.0f;
	float pitch = 1.0f;
};

struct AudioSystem : EntitySystem
{
	static sf::Box<AudioSystem> create(const SystemsDesc &desc);

	virtual void playOneShot(const sp::SoundRef &sound, const AudioInfo &info) = 0;

	virtual void addSound(Systems &systems, uint32_t entityId, uint8_t componentIndex, const sv::SoundComponent &c, const Transform &transform) = 0;

	virtual void update() = 0;

	virtual void audioThreadStereo(float *dstBuf, uint32_t numSamples, uint32_t sampleRate) = 0;
};

}