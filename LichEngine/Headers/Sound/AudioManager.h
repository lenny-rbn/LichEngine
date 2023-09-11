#pragma once
#include <Resources/WrapperAudio.h>

namespace Physics
{
	class Transform;
}


namespace SoundSystem
{
	class SoundEvent;

	class LICH_ENGINE AudioManager 
	{
	public:
		AudioManager();
		~AudioManager();

		static LichSoundEngine* engine;
		static void SetListnerPosition(Physics::Transform* transform);
		static void PlaySound(SoundEvent* soundEvent);
		static void StopSound(SoundEvent* soundEvent);
	};
}