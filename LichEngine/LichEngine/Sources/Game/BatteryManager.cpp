#include <pch.h>
#include <Scripts/BatteryManager.h>
#include <Generated/BatteryManager.rfks.h>

void BatteryManager::Start()
{
    batteries = transform->GetChildrenArray();
    pressText = gameObject->GetSceneGraph()->GetGameObject("TextGrab1")->GetComponent<UI::Text>();
}

void BatteryManager::UpdateScript()
{
    if (canGrabOneBattery)
    {
        pressText->SetText(" PRESS E TO GRAB");
        canGrabOneBattery = false;
    }
    else if (!canGrabOneBattery)
    {
        pressText->SetText(" ");
    }
}

void BatteryManager::Reload()
{
    for (int i = 0; i < batteries.size(); i++)
        batteries[i]->GetGameObject()->GetComponent<Battery>()->Reload();
}

std::string BatteryManager::SaveScript()
{
    std::string content = " ,";
    return content;
}

void BatteryManager::LoadScript(std::string data)
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