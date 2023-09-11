#include <pch.h>
#include <Core/TimeManager.h>
#include <Generated/BlinkingLight.rfks.h>

void BlinkingLight::Start()
{
	light = gameObject->GetComponent<LightSystem::Light>();
	intensity = maxIntensity;
	light->SetIntensity(intensity);
	int32_t i;
	inactionTime = RandomNumber(i ,0.f, 0.7f);
}

void BlinkingLight::UpdateScript()
{
	if (onTransition)
	{
		transitionTime -= Core::TimeManager::GetDeltaTime();

		if (transitionTime <= 0.f)
		{
			onTransition = false;
			int32_t i;
			inactionTime = RandomNumber(i, 0.f, 0.7f);
		}
		else if (gettingBrighter)
		{
			intensity = minIntensity + Lerp(maxIntensity, minIntensity, transitionTime / transitionTimeTotal);
		}
		else
		{
			intensity = maxIntensity - Lerp(minIntensity, maxIntensity, transitionTime / transitionTimeTotal);
		}
	}
	else
	{
		inactionTime -= Core::TimeManager::GetDeltaTime();

		if (inactionTime <= 0.f)
		{
			onTransition = true;
			int32_t i;
			transitionTime = transitionTime = RandomNumber(i, 0.f, 0.2f);

			gettingBrighter = !gettingBrighter;
		}
		else if (gettingBrighter)
		{
			intensity = maxIntensity;
		}
		else
		{
			intensity = minIntensity;
		}
	}
	
	if (intensity <= 0.f) intensity = 0.f;

	light->SetIntensity(intensity);
}

std::string BlinkingLight::SaveScript()
{
	std::string content = std::to_string(maxIntensity) + ',';
	content += std::to_string(minIntensity) + ',';
	return content;
}

void BlinkingLight::LoadScript(std::string data)
{
	const int sizeData = 3;
	std::string dataCut[sizeData];
	size_t index = 0,baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',',baseIndex + 1);
		dataCut[i] = data.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}

	maxIntensity = std::stof(dataCut[1]);
	minIntensity = std::stof(dataCut[2]);
}