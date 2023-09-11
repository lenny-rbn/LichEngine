#pragma once
#include <Resources/WrapperAudio.h>
#include <Resources/Resource.h>
#include <Sound/AudioManager.h>
#include <filesystem>
#include <Generated/SoundInstance.rfkh.h>

namespace Resources NAMESPACE()
{
	class CLASS() LICH_ENGINE SoundInstance : public Resources::Resource
	{
	public:
		SoundInstance();
		SoundInstance(std::filesystem::path& resourcePath);
		SoundInstance(const std::string& name,const std::string& path);
		~SoundInstance();
		
		LichSoundSource* pSoundSource;

		virtual void Load(const std::string& path);
		virtual std::string Save();
		virtual bool IsCreated() { return true; };
		virtual void Unload();

		Resources_SoundInstance_GENERATED
	};
}
File_SoundInstance_GENERATED