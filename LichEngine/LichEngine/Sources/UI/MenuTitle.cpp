#include<pch.h>
#include <Core/TimeManager.h>
#include <Resources/ResourceManager.h>
#include <Generated/MenuTitle.rfks.h>


void MenuTitle::Start()
{
	m_pTextComponent = gameObject->GetComponent<UI::Text>();
	m_pTextComponent->SetOpacity(0);
}

void MenuTitle::UpdateScript()
{
	if (Resources::ResourceManager::LoadFinish())
	{
		if (!isFadeFinish)
		{
			if (fadeTimer < fadeTime)
			{
				fadeTimer += (float)Core::TimeManager::GetDeltaTime();
			}
			else
			{
				isFadeFinish = true;
			}
			m_pTextComponent->SetOpacity(0);
		}
		else
		{
			if (opacityValue < 1)
			{
				opacityValue += (float)Core::TimeManager::GetDeltaTime();
				m_pTextComponent->SetOpacity(opacityValue);
			}
		}
	}
}

std::string MenuTitle::SaveScript()
{
	std::string content = std::to_string(fadeTime) + ',';
	return content;
}

void MenuTitle::LoadScript(std::string data)
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


	fadeTime = std::stof(dataCut[1]);
}