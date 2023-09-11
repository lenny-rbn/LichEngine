#pragma once
#include <Resources/Scripting.h>
#include <Core/SpotLight.h>
#include <Sound/SoundEvent.h>
#include <Generated/Flashlight.rfkh.h>


class CLASS() LICH_ENGINE Flashlight : public Resources::Scripting
{
public:

    FIELD() float minIntensity = 0.5f;
    FIELD() float maxIntensity = 20.f;
    FIELD() float batteryDuration = 20.f;
    FIELD() float volumeSound = 1.f;


    LightSystem::SpotLight* light;
    SoundSystem::SoundEvent* sounds;
    float currentBatteryDuration;

    bool isEnable;
    float savedIntensity;

    void Start() override;
    void UpdateScript() override;
    void RechargeFlashLight();

    std::string SaveScript() override;
    void LoadScript(std::string data) override;

    Flashlight_GENERATED
};
File_Flashlight_GENERATED
