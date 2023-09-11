#include <pch.h>
#include <Core/TimeManager.h>
#include <Physics/Collider.h>
#include <Physics/Rigidbody.h>
#include <Core/InputManager.h>
#include <string>
#include <Physics/PhysicsManager.h>
#include <Generated/Monster.rfks.h>


void Monster::Start()
{
	canMove = true;
	hasCollided = false;

	monsterT = gameObject->GetTransform();
	playerT = gameObject->GetSceneGraph()->GetGameObject("Player")->GetTransform();
	animator = gameObject->GetComponent<AnimationSystem::Animator>();

	animator->PlayCurrentAnimation(true);

	soundEvent = gameObject->GetComponent<SoundSystem::SoundEvent>();
}

void Monster::ChasePlayer(Vector3D direction)
{
	Vector3D dir = Vector3D::Normalize(direction);
	Vector3D forward = monsterT->GetVectorForward();

	float angle = RAD2DEG(atan2f(dir.z * forward.x - dir.x * forward.z, dir.z * forward.z + dir.x * forward.x));
	if (angle > 0.5f || angle < -0.5f)
		monsterT->Rotate(Vector3D(0, angle, 0));

	if (screamTime > 0.f)
	{
		screamTime -= Core::TimeManager::GetDeltaTime();
		return;
	}
	else if (screamTime > -1.f)
	{
		timeBetweenTwoStep /= 2;
		animator->StartTransition(2, 0.2f);
		animator->SetAnimationSpeed(1.f);
		soundEvent->PlaySound(soundEvent->GetSoundCount() - 1);
		screamTime = -1.f;
	}
	
	float eSpeed = 3.f * speed * (float)Core::TimeManager::GetDeltaTime();
	monsterT->Translate(dir * eSpeed);

	if (soundTransitionTime >= timeBetweenTwoStep)
	{
		soundTransitionTime = 0.f;
		int count = soundEvent->GetSoundCount()-1;
		int soundId = RandomNumber(i, 0, count - 1);
		if (soundId < 0) soundId *= -1;
		soundEvent->PlaySound(soundId);
	}
	else
	{
		soundTransitionTime += Core::TimeManager::GetDeltaTime();
	}
}

void Monster::UpdatePosition()
{
	float eSpeed = speed * (float)Core::TimeManager::GetDeltaTime();

	Vector3D direction = *waypoint[waypointIndex] - monsterT->GetGlobalPosition();
	Vector3D dir = Vector3D::Normalize(direction);
	Vector3D forward = monsterT->GetVectorForward();
	
	float angle = RAD2DEG(atan2f(dir.z * forward.x - dir.x * forward.z, dir.z * forward.z + dir.x * forward.x));
	if (angle > 0.5f || angle < -0.5f)
		monsterT->Rotate(Vector3D(0, angle, 0));
	
	monsterT->Translate(dir * eSpeed);

	if (hasCollided || direction.Magnitude() <= 0.05f)
	{
		hasCollided = false;
		waypointIndex++;

		if (waypointIndex >= waypoint.size())
			waypointIndex = 0.f;
	}

	if (soundTransitionTime >= timeBetweenTwoStep)
	{
		soundTransitionTime = 0.f;
		int count = soundEvent->GetSoundCount();
		int soundId = RandomNumber(i, 0, count - 1);
		if (soundId < 0) soundId *= -1;
		soundEvent->PlaySound(soundId);
	}
	else
	{
		soundTransitionTime += Core::TimeManager::GetDeltaTime();
	}
}

void Monster::UpdateScript()
{
	if (canMove)
	{
		Vector3D IAToPlayer = playerT->GetGlobalPosition() - monsterT->GetGlobalPosition();
		if (playerLocked)
		{
			ChasePlayer(IAToPlayer);
			return;
		}

		if (IAToPlayer.Magnitude() <= detectionRadius)
		{
			Vector3D direction = *waypoint[waypointIndex] - monsterT->GetGlobalPosition();
			float angle = GetAngle(Vector2D(IAToPlayer.x, IAToPlayer.z), Vector2D(direction.x, direction.z));
			SceneManagement::GameObject* hit = Physics::Raycast(gameObject->GetPhysicsManager(), monsterT->GetGlobalPosition() + Vector3D::Normalize(IAToPlayer) + Vector3D(0, 0.2f, 0), IAToPlayer, 4.f);
			if (angle < 30.f && hit != nullptr && hit->GetName() == "Player" && !playerLocked)
			{
				animator->StartTransition(1, 0.2f);
				animator->SetAnimationSpeed(1.5f);
				playerLocked = true;
				return;
			}
		}

		UpdatePosition();	
	}
}

#pragma region Saving...
std::string Monster::SaveScript()
{
	std::string content = std::to_string(speed) + ',';

	content += std::to_string(waypoint.size()) + ',';
	for (int i = 0; i < waypoint.size(); i++)
		content += waypoint[i]->ToString() + ',';

	return content;
}

void Monster::LoadScript(std::string data)
{
	const int sizeData = 3;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	speed = std::stof(dataCut[1]);

	for (size_t i = 0; i < std::stoi(dataCut[2]); i++)
	{
		index = data.find(',', baseIndex + 1);
		Vector3D vector = Vector3D::GetFromString(data.substr(baseIndex, index - baseIndex));
		waypoint.push_back(new Vector3D(vector));
		baseIndex = index + 1;
	}
}
#pragma endregion