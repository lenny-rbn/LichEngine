#pragma once
#include <Resources/Scripting.h>
#include <UI/Image.h>
#include <Generated/ImageFade.rfkh.h>

class CLASS() LICH_ENGINE ImageFade : public Resources::Scripting
{
public:
	FIELD() float fadeTime = 0.f;
	void Start() override;
	void UpdateScript() override;

	bool Started() override { return started; }

	void ChangeFadeState();

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

private:
	float m_fadeTimer = 0.0f;
	bool m_isLaunch = false;
	bool m_isFadeOut = true;

	UI::Image* m_pImageBackground = nullptr;

	ImageFade_GENERATED
};
File_ImageFade_GENERATED