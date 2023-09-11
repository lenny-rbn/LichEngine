#pragma once
#include <Resources/Scripting.h>
#include <UI/Text.h>
#include <Generated/MenuTitle.rfkh.h>

class CLASS() LICH_ENGINE MenuTitle : public Resources::Scripting
{
public:
	void Start() override;
	void UpdateScript() override;
	FIELD() float fadeTime = 1.5f;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	UI::Text* m_pTextComponent;
private:
	float fadeTimer = 0;
	float opacityValue = 0;
	bool isFadeFinish = false;

	MenuTitle_GENERATED
};

File_MenuTitle_GENERATED