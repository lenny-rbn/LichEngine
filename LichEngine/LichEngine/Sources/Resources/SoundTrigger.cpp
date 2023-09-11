#include <pch.h>
#include <Physics/Collider.h>
#include <Generated/SoundTrigger.rfks.h>

void SoundTrigger::Start()
{
	soundEvent = gameObject->GetComponent<SoundSystem::SoundEvent>();
	
	if (!gameObject->HasComponent<Physics::Collider>())
		gameObject->AddComponent<Physics::Collider>();

	Physics::Collider* coll = gameObject->AddComponent<Physics::Collider>();
	coll->SetTriggerState(true);
}

void SoundTrigger::OnTriggerEnter(SceneManagement::GameObject* other)
{
	if (!hasBeenTrigger)
	{
		soundEvent->PlaySound(0);
		hasBeenTrigger = true;
	}
}