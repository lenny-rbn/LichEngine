#pragma once
#include <Resources/Scripting.h>
#include <Scripts/Battery.h>
#include <Generated/BatteryManager.rfkh.h>


class CLASS() LICH_ENGINE BatteryManager : public Resources::Scripting
{
public:

    std::vector<Physics::Transform*> batteries;
    bool canGrabOneBattery;
    UI::Text* pressText;

    void Start() override;
    void UpdateScript() override;
    void Reload();

    std::string SaveScript() override;
    void LoadScript(std::string data) override;

    BatteryManager_GENERATED
};
File_BatteryManager_GENERATED