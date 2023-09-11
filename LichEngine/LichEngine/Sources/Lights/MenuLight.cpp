#include <pch.h>
#include <Core/TimeManager.h>
#include <Resources./ResourceManager.h>
#include <cmath>
#include <Generated/MenuLight.rfks.h>

void MenuLight::Start()
{
	additionalTime = true;
	light = gameObject->GetComponent<LightSystem::Light>();
	light->SetIntensity(minIntensity);
}

void MenuLight::UpdateScript()
{
	if (!isFadeFinish && Resources::ResourceManager::LoadFinish())
	{
		if (!additionalTime)
		{

			float intensity = minIntensity + maxIntensity * 0.5f * exp(2.5f * fadeTimer - 7);
			light->SetIntensity(intensity);
			if (fadeTimer < fadeTime)
			{
				fadeTimer += (float)Core::TimeManager::GetDeltaTime();
			}
			else
			{
				isFadeFinish = true;
			}


		}
		else
		{

			if (fadeTimer < additionalTime)
			{
				fadeTimer += (float)Core::TimeManager::GetDeltaTime();
			}
			else
			{
				additionalTime = false;
				fadeTimer = 0;
			}
		}

	}
}

std::string MenuLight::SaveScript()
{
	std::string content = std::to_string(maxIntensity) + ',';
	content += std::to_string(minIntensity) + ',';
	content += std::to_string(fadeTime) + ',';
	content += std::to_string(additionalTime) + ',';
	return content;
}

void MenuLight::LoadScript(std::string data)
{
	const int sizeData = 5;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	maxIntensity = std::stof(dataCut[1]);
	minIntensity = std::stof(dataCut[2]);
	fadeTime = std::stof(dataCut[3]);
	additionalTime = std::stof(dataCut[4]);
}