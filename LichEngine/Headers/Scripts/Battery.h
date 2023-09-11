#pragma once
#include <Resources/Scripting.h>
#include <Core/SpotLight.h>
#include <Scripts/Flashlight.h>
#include <UI/Text.h>
#include <Generated/Battery.rfkh.h>


class CLASS() LICH_ENGINE Battery : public Resources::Scripting
{
public:

    FIELD() float distanceToGrab;
    FIELD() float angleToGrab;

    bool hasBeenUsed;
    Flashlight* flashlight;
    Physics::Transform* eyeTransform;

    void Start() override;
    void UpdateScript() override;
    void Reload();

    std::string SaveScript() override;
    void LoadScript(std::string data) override;

    Battery_GENERATED
};
File_Battery_GENERATED