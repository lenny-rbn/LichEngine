#pragma once
#include <Resources/Scripting.h>
#include <Core/Light.h>
#include <Generated/BlinkingLight.rfkh.h>


class CLASS() LICH_ENGINE BlinkingLight : public Resources::Scripting
{
public:
	LightSystem::Light* light;

	FIELD() float maxIntensity = 20.f;
	FIELD() float minIntensity = 0.f;

	float intensity = 0.f;
	float inactionTime = 0.f;
	float transitionTime = 0.f;
	float transitionTimeTotal = 0.f;

	bool onTransition = false;
	bool gettingBrighter = false;

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;


	BlinkingLight_GENERATED
};
File_BlinkingLight_GENERATED