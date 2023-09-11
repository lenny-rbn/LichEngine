#pragma once
#include <Resources/Scripting.h>
#include <Animation/Animator.h>
#include <Sound/SoundEvent.h>

#include <Generated/Monster.rfkh.h>


class CLASS() LICH_ENGINE Monster : public Resources::Scripting
{
public:
	FIELD() float speed = 2.5f;
	FIELD() VectorContainer<Vector3D> waypoint;

	bool canMove;
	bool hasCollided;
	bool animationChange = false;
	bool playerLocked = false;
	float screamTime = 0.9f;
	int waypointIndex = 0;

	int32_t i = 0;
	float soundTransitionTime = 0.f;
	float timeBetweenTwoStep = 0.8f;

	float detectionRadius = 4.f;

	Physics::Transform* playerT;
	Physics::Transform* monsterT;
	AnimationSystem::Animator* animator;
	SoundSystem::SoundEvent* soundEvent;

	void ChasePlayer(Vector3D direction);
	void UpdatePosition();

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	Monster_GENERATED
};
File_Monster_GENERATED