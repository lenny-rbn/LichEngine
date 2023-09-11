#pragma once
#include <Resources/Scripting.h>
#include <Scripts/Tool.h>
#include <Generated/ToolManager.rfkh.h>


class CLASS() LICH_ENGINE ToolManager : public Resources::Scripting
{
public:

    FIELD() float volumeHitSound;

    std::vector<Physics::Transform*> tools;

    void Start() override;
    void UpdateScript() override;
    void Reload();

    std::string SaveScript() override;
    void LoadScript(std::string data) override;

    ToolManager_GENERATED
};
File_ToolManager_GENERATED