#pragma once
#include <Resources/Scripting.h>
#include <UI/Text.h>
#include <Sound/SoundEvent.h>
#include <Generated/Menu.rfkh.h>


class CLASS() LICH_ENGINE MenuGame : public Resources::Scripting
{
public:
	MenuGame();
	~MenuGame() {};

	void Start() override;
	void UpdateScript() override;
	void OnTriggerEnter(SceneManagement::GameObject * other) override;
	void OnTriggerExit(SceneManagement::GameObject * other) override;
	void OnCollisionStay(SceneManagement::GameObject * other) override;
	void OnCollisionExit(SceneManagement::GameObject * other) override;
	void OnCollisionEnter(SceneManagement::GameObject * other) override;

	bool Started() override { return started; }

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	FIELD()	float fadeTime = 3.1f;

private:
	UI::Text* m_pTextComponent;
	SoundSystem::SoundEvent* m_pSoundEvent;
	bool fadeOut =true;
	float opacityValue = 0;
	float fadeTimer = 0.0f;
	bool isHide = true;


	MenuGame_GENERATED
};
File_Menu_GENERATED

