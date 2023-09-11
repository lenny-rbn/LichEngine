#include <pch.h>
#include <Core/TimeManager.h>
#include <Scripts/Flashlight.h>
#include <Generated/Flashlight.rfks.h>

void Flashlight::Start()
{
    light = gameObject->GetComponent<LightSystem::SpotLight>();
    currentBatteryDuration = batteryDuration;
    isEnable = true;
    sounds = gameObject->GetComponent<SoundSystem::SoundEvent>();
    sounds->GetSound(0)->setDefaultVolume(volumeSound);
    sounds->GetSound(1)->setDefaultVolume(volumeSound);
    sounds->GetSound(2)->setDefaultVolume(volumeSound);
}

void Flashlight::UpdateScript()
{
    if (isEnable)
    {
        if (currentBatteryDuration > 0.f)
        {
            currentBatteryDuration -= Core::TimeManager::GetDeltaTime();
            light->SetIntensity(Lerp<float>(minIntensity, maxIntensity, currentBatteryDuration / batteryDuration));
        }
    }

    if (input->IsPressed("F"))
    {
        if (!isEnable)
        {
            sounds->PlaySound(0);
            light->SetIntensity(savedIntensity);
            isEnable = true;
        }
        else
        {
            sounds->PlaySound(1);
            savedIntensity = light->GetIntensity();
            light->SetIntensity(0.f);
            isEnable = false;
        }
    }

}

void Flashlight::RechargeFlashLight()
{
    sounds->PlaySound(2);
    light->SetIntensity(maxIntensity);
    isEnable = true;
    currentBatteryDuration = batteryDuration;
}

std::string Flashlight::SaveScript()
{
    std::string content = std::to_string(maxIntensity) + ",";
    content += std::to_string(minIntensity) + ",";
    content += std::to_string(batteryDuration) + ",";
    content += std::to_string(volumeSound) + ",";
    return content;
}

void Flashlight::LoadScript(std::string data)
{
    const int sizeData = 5;
    std::string dataCut[sizeData];
    size_t index = 0, baseIndex = 0;

    for (size_t i = 0; i < sizeData; i++)
    {
        index = data.find(',', baseIndex + 1);
        dataCut[i] = data.substr(baseIndex, index - baseIndex);
        baseIndex = index + 1;
    }

    maxIntensity = std::stof(dataCut[1]);
    minIntensity = std::stof(dataCut[2]);
    batteryDuration = std::stof(dataCut[3]);
    volumeSound = std::stof(dataCut[4]);

}