#include <pch.h>
#include <Scripts/PlayMusic.h>
#include <Sound/SoundEvent.h>
#include <Generated/PlayMusic.rfks.h>

void PlayMusic::Start()
{
	SoundSystem::SoundEvent* sound = gameObject->GetComponent<SoundSystem::SoundEvent>();
	sound->GetSound(0)->setDefaultVolume(volume);
	sound->PlaySound(0);
}


std::string PlayMusic::SaveScript()
{
	std::string content = std::to_string(volume) + ',';
	return content;
}

void PlayMusic::LoadScript(std::string data)
{
	const int sizeData = 2;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	volume = std::stof(dataCut[1]);

}