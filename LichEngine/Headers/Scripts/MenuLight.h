#pragma once
#include <Resources/Scripting.h>
#include <Core/Light.h>
#include <Generated/MenuLight.rfkh.h>

class CLASS() LICH_ENGINE MenuLight : public Resources::Scripting
{
public:
	LightSystem::Light * light;
	FIELD() float maxIntensity = 20.f;
	FIELD() float minIntensity = 0.f;
	FIELD() float fadeTime = 1.5f;
	FIELD() float additionalTime = 1.0f;

	float fadeTimer = 0.0f;
	bool isFadeFinish = false;
	bool isAdditionalTine = true;

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	MenuLight_GENERATED
};
File_MenuLight_GENERATED