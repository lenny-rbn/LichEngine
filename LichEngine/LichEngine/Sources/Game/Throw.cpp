#include <pch.h>
#include <Scripts/Throw.h>
#include <Core/InputManager.h>
#include <Physics/Collider.h>
#include <Physics/Rigidbody.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include <Scripts/Tool.h>
#include <Generated/Throw.rfks.h>

void Throw::Start()
{
	toolsParent = gameObject->GetSceneGraph()->GetGameObject("ToolManager")->GetTransform();
	pressText = gameObject->GetSceneGraph()->GetGameObject("TextGrab")->GetComponent<UI::Text>();
	tools = toolsParent->GetChildrenArray();
	currentTool = nullptr;
}


void Throw::UpdateScript()
{
	if(currentTool == nullptr)
		TryGetTool();

	if (currentTool != nullptr)
		TryThrowTool();
}

void Throw::TryGetTool()
{
	bool canBeGrabbed = false;

	for (std::vector<Physics::Transform*>::iterator it = tools.begin(); it != tools.end(); it++)
	{

		Vector3D direction = (*it)->GetGlobalPosition() - transform->GetParent()->GetGlobalPosition();
		Vector3D view = transform->GetParent()->GetVectorForward();
		float distance = direction.Magnitude();
		float angle = GetAngle(Vector3D::Normalize(direction), Vector3D::Normalize(view));

		if (distance <= distanceToGrab && angle <= angleToGrab)
		{
			canBeGrabbed = true;

			if (input->IsPressed("E"))
			{
				currentTool = (*it);
				Physics::Collider* collider = (*it)->GetGameObject()->GetComponent<Physics::Collider>();
				if(collider != nullptr)
					collider->isEnable = false;

				currentTool->RemoveParent(true);
				currentTool->AddParent(transform->GetParent());
				currentTool->SetLocalPosition(Vector3D(0.1f, -0.075f, 0.1f));
				currentTool->SetLocalRotation(Vector3D(-142.f, 2.0f, 180.f));
				currentTool->SetLocalScale(Vector3D(0.05f, 0.05f, 0.05f));
				pressText->SetText("");
				Tool* tool = currentTool->GetGameObject()->GetComponent<Tool>();
				currentTool->GetGameObject()->GetComponent<SoundSystem::SoundEvent>()->PlaySound(tool->idSoundGetTool);
				return;
			}
		}
	}

	if(canBeGrabbed)
		pressText->SetText(" PRESS E TO GRAB");
	else if(!canBeGrabbed)
		pressText->SetText("");
}

void Throw::TryThrowTool()
{
	if (input->IsPressed("Mouse0"))
	{
		currentTool->GetGameObject()->GetComponent<Tool>()->hasBeenThrow = true;

		Vector3D globalPosition = transform->GetParent()->GetGlobalPosition();
		Quaternion globalRotation = currentTool->GetGlobalRotationInQuaternion();
		Vector3D directionThrow = transform->GetParent()->GetVectorForward();

		currentTool->RemoveParent(true);
		currentTool->AddParent(toolsParent);
		currentTool->SetGlobalPosition(globalPosition + directionThrow * 0.4f);
		currentTool->SetGlobalRotation(globalRotation);
		currentTool->SetLocalScale(Vector3D(0.1f, 0.1f, 0.1f));

		Physics::Collider* collider = currentTool->GetGameObject()->GetComponent<Physics::Collider>();
		if (collider != nullptr)
			collider->isEnable = true;

		Physics::RigidBody* rb = currentTool->GetGameObject()->GetComponent<Physics::RigidBody>();
		rb->AddForce(directionThrow * forceThrow, Physics::ForceMode::VELOCITY_CHANGE);
		currentTool = nullptr;
	}

}

void Throw::Reload()
{
	if (currentTool != nullptr)
	{
		currentTool->RemoveParent(true);
		currentTool->AddParent(toolsParent);
		Physics::Collider* collider = currentTool->GetGameObject()->GetComponent<Physics::Collider>();
		if (collider != nullptr)
			collider->isEnable = true;
		currentTool = nullptr;
	}
}

std::string Throw::SaveScript()
{
	std::string content = std::to_string(distanceToGrab) + ',';
	content += std::to_string(forceThrow) + ',';
	content += std::to_string(angleToGrab) + ',';

	return content;
}

void Throw::LoadScript(std::string data)
{
	const int sizeData = 4;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	distanceToGrab = std::stof(dataCut[1]);
	forceThrow = std::stof(dataCut[2]);
	angleToGrab = std::stof(dataCut[3]);
}