#include <pch.h>
#include <Scripts/Tool.h>
#include <Scripts/ToolManager.h>
#include <Physics/Collider.h>
#include <Generated/Tool.rfks.h>

void Tool::Start()
{
    startPosition = transform->GetGlobalPosition();
    startRotation = transform->GetGlobalRotationInQuaternion();
    startScale = transform->GetGlobalScale();
    soundHit = gameObject->GetComponent<SoundSystem::SoundEvent>();
    float volume = transform->GetParent()->GetGameObject()->GetComponent<ToolManager>()->volumeHitSound;
    soundHit->GetSound(0)->setDefaultVolume(volume);
    soundHit->GetSound(1)->setDefaultVolume(volume);
    soundHit->GetSound(2)->setDefaultVolume(volume);
    soundHit->GetSound(3)->setDefaultVolume(volume);
    idSoundGetTool = 3;

    hasBeenThrow = false;
    seed = GetSeed();
}

void Tool::UpdateScript()
{

}

void Tool::Reload()
{
    transform->SetGlobalPosition(startPosition);
    transform->SetGlobalRotation(startRotation);
    transform->SetGlobalScale(startScale);
}

void Tool::OnCollisionEnter(SceneManagement::GameObject* other)
{
    if (hasBeenThrow && other->GetName() != "Player")
    {
        hasBeenThrow = false;
        int count = soundHit->GetSoundCount();
        int soundId = RandomNumber(seed, 0, count);
        if (soundId < 0) soundId *= -1;
        soundHit->PlaySound(soundId);
    }
}

std::string Tool::SaveScript()
{
    std::string content = " ,";
    return content;
}

void Tool::LoadScript(std::string data)
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
}