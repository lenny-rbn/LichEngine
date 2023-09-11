#pragma once
#include <Resources/Scripting.h>
#include <Scene/GameObject.h>
#include <Sound/SoundEvent.h>
#include <Generated/Move.rfkh.h>


class CLASS() LICH_ENGINE Move : public Resources::Scripting
{
public:
	FIELD() float speed = 1.f;
	FIELD() float maxSpeed = 1.f;
	FIELD() float maxCrouchSpeed = 0.5f;
	FIELD() float timeBetweenTwoStep = 0.5f;

	float oldY;
	float newY;
	float soundTransitionTime = 0.f;
	bool isCrouch;

	std::string toolManagerName;
	std::string batteryManagerName;

	bool isAlive;
	bool isMoving = false;

	int32_t i = 0;

	Physics::Transform* playerT;
	Physics::Transform* eyeT;

	Physics::RigidBody* playerRB;
	Physics::Collider* collider;
	SoundSystem::SoundEvent* soundEvent;
	SceneManagement::GameObject* Ui;

	Flashlight* flashlight;
	Throw* throwTool;

	Vector3D initPos;
	Quaternion initRot;

	void UpdatePosition();
	void BeginCrouch();
	void EndCrouch();
	void Death();


	void Start() override;
	void UpdateScript() override;
	void FixedUpdateScript() override;

	void OnCollisionStay(SceneManagement::GameObject* other) override;
	void OnCollisionExit(SceneManagement::GameObject* other) override;
	void OnCollisionEnter(SceneManagement::GameObject* other) override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	Move_GENERATED
};
File_Move_GENERATED