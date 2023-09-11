#include <pch.h>
#include <Resources/ResourceManager.h>
#include <Physics/Transform.h>
#include <Debugs/Log.h>
#include <Generated/SoundEvent.rfks.h>

SoundSystem::SoundEvent::SoundEvent(SceneManagement::GameObject* go)
{
	componentType = SceneManagement::ComponentType::SOUNDEVENT;
	p_pGameObject = go;
	m_soundArray.clear();
}

SoundSystem::SoundEvent::~SoundEvent()
{
}

void SoundSystem::SoundEvent::Update(Physics::Transform& transform)
{
	m_soundPosition = transform.GetGlobalPosition();

	if (transform.IsPlaying() && !m_started && isPlayAsStart)
	{
		PlaySound(m_currentIndex);
		m_started = true;
	}

	if (!transform.IsPlaying() && !m_stoped)
	{
		for (int i = 0; i < m_soundArray.size(); i++)
		{
			if(m_soundArray[i] != nullptr)
				StopSound(i);

		}

		m_stoped = true;
	}

}

void SoundSystem::SoundEvent::LoadData(std::string data)
{

	const int sizeData = 7;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	
	is3D = std::stof(dataCut[0]);
	isLoop = std::stof(dataCut[1]);
	isPlayAsStart = std::stof(dataCut[2]);
	m_distanceMin = std::stof(dataCut[3]);
	m_distanceMax = std::stof(dataCut[4]);
	m_soundPosition.FromString(dataCut[5]);

	unsigned int size = std::stoi(dataCut[6]);
	m_soundArray.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		index = data.find(',', baseIndex + 1);
		std::string name = data.substr(baseIndex, index - baseIndex);
		SetSound(name.c_str(),i);
		baseIndex = index + 1;
	}
	
}

void SoundSystem::SoundEvent::AddSound(const char* name)
{
	if (Resources::ResourceManager::HasResource<Resources::SoundInstance>(name))
	{
		m_soundArray.push_back(Resources::ResourceManager::GetResource<Resources::SoundInstance>(name));
	}
}

std::string SoundSystem::SoundEvent::SaveData()
{
	std::string content = "~10,";

	content += std::to_string(is3D) + ",";
	content += std::to_string(isLoop) + ",";
	content += std::to_string(isPlayAsStart) + ",";
	content += std::to_string(m_distanceMin) + ",";
	content += std::to_string(m_distanceMax) + ",";
	content += m_soundPosition.ToString() + ",";
	content += std::to_string(m_soundArray.size()) + ",";

	for (size_t i = 0; i < m_soundArray.size(); i++)
	{
		if (m_soundArray[i] != nullptr)
			content += m_soundArray[i]->name + ",";
		else
			content += " ,";
	}


	return content;
}

void SoundSystem::SoundEvent::ApplyEditorChange()
{
}

bool SoundSystem::SoundEvent::IsValidIndex(int index)
{
	if (index < 0 || index >= m_soundArray.size())
	{
		LERROR("Not valid index");
		return false;
	}

	return true;
}

void SoundSystem::SoundEvent::SetSound(const char* name, int index)
{
	if (!IsValidIndex(index)) return;

	if (Resources::ResourceManager::HasResource<Resources::SoundInstance>(name))
	{
		m_soundArray[index] = Resources::ResourceManager::GetResource<Resources::SoundInstance>(name);
	}
}

void SoundSystem::SoundEvent::PlaySound(int index)
{
	if (!IsValidIndex(index)) return;

	if (m_soundArray.size() == 0) return;
	m_currentIndex = index;
	AudioManager::PlaySound(this);
}

void SoundSystem::SoundEvent::StopSound(int index)
{
	if (!IsValidIndex(index)) return;
	if (m_soundArray.size() == 0)return;

	m_currentIndex = index;

	AudioManager::StopSound(this);
}

void SoundSystem::SoundEvent::SetupSoundMinimunDistance(float distance)
{
	m_distanceMin = distance;
}
void SoundSystem::SoundEvent::SetupSoundMaximumDistance(float distance)
{
	m_distanceMax = distance;
}
