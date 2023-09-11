#pragma once
#include <Resources/Scripting.h>
#include <Sound/SoundEvent.h>
#include <Generated/Tool.rfkh.h>


class CLASS() LICH_ENGINE Tool : public Resources::Scripting
{
public:

    int idSoundGetTool;
    int32_t seed;
    bool hasBeenThrow;
    bool OnGround;
    SoundSystem::SoundEvent* soundHit;
    Vector3D startPosition;
    Quaternion startRotation;
    Vector3D startScale;

    void Start() override;
    void UpdateScript() override;
    void OnCollisionEnter(SceneManagement::GameObject* other) override;
    void Reload();

    std::string SaveScript() override;
    void LoadScript(std::string data) override;

    Tool_GENERATED
};
File_Tool_GENERATED