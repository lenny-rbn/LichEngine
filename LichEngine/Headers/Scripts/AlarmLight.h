#pragma once
#include <Resources/Scripting.h>
#include <Core/SpotLight.h>
#include <Generated/AlarmLight.rfkh.h>


class CLASS() LICH_ENGINE AlarmLight : public Resources::Scripting
{
public:
	LightSystem::SpotLight* light;

	FIELD() float intensity = 20.f;
	FIELD() float rotatingSpeed = 20.f;
	FIELD() bool isActivated = false;

	void Start() override;
	void UpdateScript() override;
	void ChangeActivateState() { isActivated = !isActivated; }

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	AlarmLight_GENERATED
};
File_AlarmLight_GENERATED