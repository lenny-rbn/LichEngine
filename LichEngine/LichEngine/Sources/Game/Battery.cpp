#include <pch.h>
#include <Core/TimeManager.h>
#include <Scripts/Battery.h>
#include <Scripts/BatteryManager.h>
#include <Renderer/Mesh.h>
#include <Generated/Battery.rfks.h>

void Battery::Start()
{
    flashlight = (Flashlight*)gameObject->GetSceneGraph()->GetGameObject("FlashLight")->GetComponent<Resources::Scripting>();
    eyeTransform = gameObject->GetSceneGraph()->GetGameObject("Eye")->GetTransform();
    hasBeenUsed = false;
}

void Battery::UpdateScript()
{
    if (flashlight != nullptr && eyeTransform != nullptr && !hasBeenUsed)
    {
        Vector3D directionPlayerBattery = transform->GetGlobalPosition() - eyeTransform->GetGlobalPosition();
        float distancePlayerBattery = directionPlayerBattery.Magnitude();
        float angle = GetAngle(eyeTransform->GetVectorForward(), Vector3D::Normalize(directionPlayerBattery));

        if (distancePlayerBattery <= distanceToGrab && angle <= angleToGrab)
        {
            transform->GetParent()->GetGameObject()->GetComponent<BatteryManager>()->canGrabOneBattery = true;
            if (input->IsPressed("E"))
            {
                hasBeenUsed = true;
                flashlight->RechargeFlashLight();
                gameObject->GetComponent<Renderer::Mesh>()->isEnable = false;
            }

        }
    }
}

void Battery::Reload()
{
    if (hasBeenUsed)
    {
        hasBeenUsed = false;
        gameObject->GetComponent<Renderer::Mesh>()->isEnable = true;
    }
}

std::string Battery::SaveScript()
{
    std::string content = std::to_string(distanceToGrab) + ",";
    content += std::to_string(angleToGrab) +  ",";

    return content;
}

void Battery::LoadScript(std::string data)
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

    distanceToGrab = std::stof(dataCut[1]);
    angleToGrab = std::stof(dataCut[2]);
}