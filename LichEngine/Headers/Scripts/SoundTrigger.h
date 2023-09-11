#pragma once
#include <Resources/Scripting.h>
#include <Sound/SoundEvent.h>
#include <Generated/SoundTrigger.rfkh.h>


class CLASS() LICH_ENGINE SoundTrigger : public Resources::Scripting
{
public:
	SoundSystem::SoundEvent* soundEvent;
	bool hasBeenTrigger = false;

	void Start() override;

	void OnTriggerEnter(SceneManagement::GameObject* other) override;

	SoundTrigger_GENERATED
};
File_SoundTrigger_GENERATED