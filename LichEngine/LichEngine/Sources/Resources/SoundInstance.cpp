#include <pch.h>
#include <Debugs/Log.h>
#include <Generated/SoundInstance.rfks.h>

namespace Resources
{
	SoundInstance::SoundInstance() 
	{
		type = ResourceType::SOUND;
	}

	SoundInstance::SoundInstance(std::filesystem::path &resourcePath)
	{
		path = resourcePath.string();
		std::string filename = resourcePath.filename().string();
		name  = filename.substr(0,filename.find_first_of('.'));
		type = ResourceType::SOUND;
	}

	SoundInstance::SoundInstance(const std::string& name,const std::string& path)
	{
		this->path = path;
		this->name = name;
		type = ResourceType::SOUND;
	}

	SoundInstance::~SoundInstance() {}

	void SoundInstance::Load(const std::string& path)
	{
		pSoundSource = AudioAPI::AddSoundSource(SoundSystem::AudioManager::engine, path.c_str());
	}

	void SoundInstance::Unload()
	{
		AudioAPI::RemoveSoundSource(SoundSystem::AudioManager::engine,pSoundSource);
	}

	std::string SoundInstance::Save()
	{
		std::string content = "4,";
		content += path + ',';
		content += name + ',';
		return content;
	}


	
}