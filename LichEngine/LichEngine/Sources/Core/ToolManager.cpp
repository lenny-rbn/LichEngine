#include <pch.h>
#include <Scripts/ToolManager.h>
#include <Generated/ToolManager.rfks.h>

void ToolManager::Start()
{
    tools = gameObject->GetTransform()->GetChildrenArray();

}

void ToolManager::UpdateScript()
{

}

void ToolManager::Reload()
{
    for (int i = 0; i < tools.size(); i++)
    {
        tools[i]->GetGameObject()->GetComponent<Tool>()->Reload();

        //If the tool is in the hand of the player
        if (tools[i]->GetParent() != transform) 
        {
            tools[i]->RemoveParent(true);
            tools[i]->AddParent(transform);
        }
    }
}

std::string ToolManager::SaveScript()
{
    std::string content = std::to_string(volumeHitSound) + ',';
    return content;
}

void ToolManager::LoadScript(std::string data)
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

    volumeHitSound = std::stof(dataCut[1]);

}