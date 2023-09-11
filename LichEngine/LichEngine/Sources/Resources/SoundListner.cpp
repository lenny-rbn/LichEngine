#include "pch.h"
#include <Generated/SoundListner.rfks.h>



SoundSystem::SoundListner::SoundListner(SceneManagement::GameObject* go)
{ 
	componentType = SceneManagement::ComponentType::SOUNDLISTNER;
	p_pGameObject = go;
}

SoundSystem::SoundListner::~SoundListner()
{
}


void SoundSystem::SoundListner::LoadData(std::string data)
{

}

std::string SoundSystem::SoundListner::SaveData()
{
	std::string content = "~9,";
	return content;
}

void SoundSystem::SoundListner::ApplyEditorChange()
{
}

void SoundSystem::SoundListner::Update(Physics::Transform& transform)
{
	AudioManager::SetListnerPosition(&transform);
}