#include <pch.h>
#include <Debugs/Log.h>
#include <Sound/AudioManager.h>
#include <Physics/Transform.h>
#include <Sound/SoundEvent.h>

namespace SoundSystem
{
	LichSoundEngine* AudioManager::engine = nullptr;

	AudioManager::AudioManager()
	{
		engine = irrklang::createIrrKlangDevice();

		if (!engine)
			LDEBUG("Failed to create IrrKlang's device");
	}

	AudioManager::~AudioManager()
	{
		delete engine;
	}

	void AudioManager::SetListnerPosition(Physics::Transform* transform) 
	{
		engine->setListenerPosition(transform->GetGlobalPosition(),transform->GetVectorForward(),transform->GetVectorUp());
	}

	void AudioManager::PlaySound(SoundEvent* soundEvent)
	{

		if (soundEvent->is3D)
		{
			soundEvent->GetSound()->setDefaultMinDistance(soundEvent->GetMinDistance());
			soundEvent->GetSound()->setDefaultMaxDistance(soundEvent->GetMaxDistance());
			engine->play3D(soundEvent->GetSound(),soundEvent->GetPosition(),soundEvent->isLoop,false);
		}
		else
		{
			engine->play2D(soundEvent->GetSound(),soundEvent->isLoop,false);
		}

	}

	void AudioManager::StopSound(SoundEvent* soundEvent)
	{
	
		engine->stopAllSoundsOfSoundSource(soundEvent->GetSound());
	}

}