#pragma once

#include <Resources/Scripting.h>
#include <Generated/PauseMenu.rfkh.h>

class CLASS() PauseMenu : public Resources::Scripting
{
public:
	void Start() override;
	void UpdateScript() override;

	bool Started() override { return started; }

	std::string SaveScript() override;
	void LoadScript(std::string data) override;
private:
	PauseMenu_GENERATED
};
File_PauseMenu_GENERATED




