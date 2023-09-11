#pragma once
#include <Resources/Scripting.h>
#include <Generated/Gameplay.rfkh.h>


class CLASS() LICH_ENGINE Gameplay : public Resources::Scripting
{
public:
	FIELD() int hp = 0;
	FIELD() float speed = 0.f;
	FIELD() float jumpForce = 100.f;

	bool canJump = false;
	bool isJumping = false;

	void Move();
	void Jump();
	void CheckInput();
	void ResetPlayer();

	void Start() override;
	void UpdateScript() override;
	void OnTriggerEnter(SceneManagement::GameObject* other) override;
	void OnTriggerExit(SceneManagement::GameObject* other) override;
	void OnCollisionStay(SceneManagement::GameObject* other) override;
	void OnCollisionExit(SceneManagement::GameObject* other) override;
	void OnCollisionEnter(SceneManagement::GameObject* other) override;

	bool Started() override { return started; }

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	Gameplay_GENERATED
};
File_Gameplay_GENERATED