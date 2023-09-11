#pragma once
#include <Resources/Scripting.h>
#include <Generated/PlayMusic.rfkh.h>


class CLASS() LICH_ENGINE PlayMusic : public Resources::Scripting
{
public:

	FIELD() float volume;

	void Start() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	PlayMusic_GENERATED
};
File_PlayMusic_GENERATED