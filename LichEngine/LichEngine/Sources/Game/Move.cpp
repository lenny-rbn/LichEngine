#include <pch.h>
#include <Core/TimeManager.h>
#include <Physics/Collider.h>
#include <Physics/Rigidbody.h>
#include <Core/InputManager.h>
#include <Core/SpotLight.h>
#include <Scripts/ToolManager.h>
#include <Scripts/BatteryManager.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include <Scripts/Throw.h>

#include <Core/SpotLight.h>
#include <Generated/Move.rfks.h>


void Move::Start()
{
	playerT = gameObject->GetTransform();
	initPos = playerT->GetGlobalPosition();
	initRot = playerT->GetGlobalRotationInQuaternion();

	if(!gameObject->HasSpecificComponent(5))
		gameObject->AddSpecificComponent(5);

	playerRB = gameObject->GetComponent<Physics::RigidBody>();
	collider = gameObject->GetComponent<Physics::Collider>();
	soundEvent = gameObject->GetComponent<SoundSystem::SoundEvent>();

	eyeT = gameObject->GetTransform()->GetChildrenArray()[0];
	flashlight = eyeT->GetChildrenArray()[0]->GetGameObject()->GetComponent<Flashlight>();
	throwTool = eyeT->GetChildrenArray()[1]->GetGameObject()->GetComponent<Throw>();

	Ui = gameObject->GetSceneGraph()->GetGameObject("UIMenu");

	toolManagerName = "ToolManager";
	batteryManagerName = "BatteryManager";

	isAlive = true;

	soundTransitionTime = timeBetweenTwoStep;
	i = GetSeed();

	for (int soundID = 0; soundID < soundEvent->GetSoundCount(); soundID++)
		soundEvent->GetSound(soundID)->setDefaultVolume(0.2f);

	if (input->IsPressed("Shift"))
		isCrouch = true;
}

void Move::UpdatePosition()
{
	Vector3D direction = Vector3D::zero;
	float eSpeed = speed * (float)Core::TimeManager::GetDeltaTime();

	if (input->IsContinue("Front"))
		direction += Vector3D::Normalize(eyeT->GetVectorForward() * Vector3D::x0z);
	if (input->IsContinue("Back"))
		direction += Vector3D::Normalize(eyeT->GetVectorForward() * Vector3D::x0z) * -1.f;
	if (input->IsContinue("Left"))
		direction += Vector3D::Normalize(eyeT->GetVectorRight());
	if (input->IsContinue("Right"))
		direction += Vector3D::Normalize(eyeT->GetVectorRight() * -1.f);

	if (direction.Magnitude() != 0)
	{
		direction.Normalize();
		playerT->Translate(direction * eSpeed);
		isMoving = true;
	}
	else
	{
		playerRB->SetVelocity(playerRB->GetVelocity() * Vector3D::up);
		isMoving = false;
	}

	if (soundTransitionTime >= timeBetweenTwoStep && isMoving)
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

void Move::UpdateScript()
{
	if (isAlive)
		UpdatePosition();

	if (input->IsPressed("Escape") )
	{
		Ui->SetActive();
		Ui->UpdateState();
	}
}

void Move::FixedUpdateScript()
{
	if (input->IsPressed("Shift") && !isCrouch)
		BeginCrouch();

	if (input->IsReleased("Shift") && isCrouch)
		EndCrouch();
}

void Move::BeginCrouch()
{
	isCrouch = true;
	speed /= 2.0f;
	Vector3D eyeLocalPos = eyeT->GetLocalPosition();
	eyeLocalPos.y = 0.2f;

	Vector3D collScale = collider->GetScale();
	oldY = collScale.y;
	collScale.y /= 5.f;
	newY = collScale.y;

	collider->SetScale(collScale);
	gameObject->GetTransform()->Translate(Vector3D(0, (newY - oldY), 0));
	//eyeT->SetLocalPosition(eyeLocalPos);
	//spotlightT->SetLocalPosition(eyeLocalPos);
}

void Move::EndCrouch()
{
	isCrouch = false;
	speed *= 2.0f;
	Vector3D eyeLocalPos = eyeT->GetLocalPosition();
	eyeLocalPos.y = 0.5f;
	//eyeT->SetLocalPosition(eyeLocalPos);
	//spotlightT->SetLocalPosition(eyeLocalPos);
	Vector3D collScale = collider->GetScale();
	oldY = collScale.y;
	collScale.y *= 5.f;
	newY = collScale.y;

	gameObject->GetTransform()->Translate(Vector3D(0, (newY - oldY), 0));
	collider->SetScale(collScale);
}

void Move::Death()
{
	gameObject->GetSceneGraph()->GetScene()->GetSceneManager()->ChangeSceneInGame("Level 1.lscene", "Resources\\Level 1.lscene",true);
}

#pragma region Collision/Trigger...
void Move::OnCollisionEnter(SceneManagement::GameObject* other)
{
	if (other->GetName() == "Monster")
		Death();
}

void Move::OnCollisionStay(SceneManagement::GameObject* other)
{

}

void Move::OnCollisionExit(SceneManagement::GameObject* other)
{

}
#pragma endregion

#pragma region Saving...
std::string Move::SaveScript()
{
	std::string content = std::to_string(speed) + ',';
	content += std::to_string(maxSpeed) + ',';

	return content;
}

void Move::LoadScript(std::string data)
{
	const int sizeData = 3;
	std::string dataCut[sizeData];
	size_t index = 0,baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',',baseIndex + 1);
		dataCut[i] = data.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}

	speed = std::stof(dataCut[1]);
	maxSpeed = std::stof(dataCut[2]);

}
#pragma endregion