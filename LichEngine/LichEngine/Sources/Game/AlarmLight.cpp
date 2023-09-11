#include <pch.h>
#include <Core/TimeManager.h>
#include <Generated/AlarmLight.rfks.h>

void AlarmLight::Start()
{
	light = gameObject->GetComponent<LightSystem::SpotLight>();
}

void AlarmLight::UpdateScript()
{

	gameObject->GetTransform()->Rotate(Vector3D(0,0,rotatingSpeed * Core::TimeManager::GetDeltaTime()));
}

std::string AlarmLight::SaveScript()
{
	std::string content = std::to_string(intensity) + ',';
	content += std::to_string(rotatingSpeed) + ',';
	return content;
}

void AlarmLight::LoadScript(std::string data)
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

	intensity = std::stof(dataCut[1]);
	rotatingSpeed = std::stof(dataCut[2]);
}