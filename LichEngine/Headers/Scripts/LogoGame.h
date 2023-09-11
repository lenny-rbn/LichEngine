#pragma once
#include <Resources/Scripting.h>
#include <UI/Image.h>
#include <Generated/LogoGame.rfkh.h>

class CLASS() LICH_ENGINE LogoGame : public Resources::Scripting
{
public:

	UI::Image* imageLogo;
	FIELD() float showTime;
	
	float showTimer;
	float opacityValue;
	bool isShowing;

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	LogoGame_GENERATED

};
File_LogoGame_GENERATED


