#include <pch.h>
#include <Scripts/Move.h>
#include <Core/TimeManager.h>
#include <Core/InputManager.h>
#include <Generated/Look.rfks.h>


void Look::Start()
{
	playerT = transform->GetParent();

	oldMouseX = 0.f;
	oldMouseY = 0.f;

	started = true;
}

void Look::UpdateScript()
{
	if (playerT->GetGameObject()->GetComponent<Move>()->isAlive)
	{
		//double mouseX, mouseY;
		//WindowAPI::GetCursorPosition(input->GetWindow(), &mouseX, &mouseY);
		//Physics::Transform* transform = gameObject->GetTransform();
		//float angleY, angleX;
		//
		//angleX = (float)((mouseY - oldMouseY) * Core::TimeManager::GetDeltaTime()) * camSensitivity;
		//angleY = (float)((mouseX - oldMouseX) * Core::TimeManager::GetDeltaTime()) * -camSensitivity;
		//
		//Quaternion quatY = Quaternion::AngleAxisToQuat(transform->GetVectorUp(), angleY);
		//Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), angleX);
		//
		//float dot = DotProduct3D(transform->GetVectorForward(), Vector3D::up);
		//if ((dot <= -0.7f && angleX > 0.f) || (dot >= 0.7f && angleX < 0.f))
		//	quatX = Quaternion::Identity;
		//
		//transform->SetGlobalRotation(quatX * quatY * transform->GetGlobalRotationInQuaternion());
		//
		//oldMouseX = mouseX;
		//oldMouseY = mouseY;


		double mouseX, mouseY;
		WindowAPI::GetCursorPosition(input->GetWindow(), &mouseX, &mouseY);
		Vector3D eulerAngle = transform->GetGlobalRotation();

		eulerAngle.x += (float)((mouseY - oldMouseY) * Core::TimeManager::GetDeltaTime()) * camSensitivity;
		eulerAngle.y += (float)((mouseX - oldMouseX) * Core::TimeManager::GetDeltaTime()) * -camSensitivity;
		eulerAngle.x = Clamp(eulerAngle.x, -45.f, 60.f);

		transform->SetGlobalRotation(eulerAngle);
		oldMouseX = mouseX;
		oldMouseY = mouseY;

	}
}

std::string Look::SaveScript()
{
	std::string content = std::to_string(camSensitivity) + ',';
	return content;
}

void Look::LoadScript(std::string data)
{
	const int sizeData = 2;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	camSensitivity = std::stof(dataCut[1]);
}