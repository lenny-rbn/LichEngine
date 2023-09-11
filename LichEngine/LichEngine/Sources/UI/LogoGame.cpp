#include <pch.h>
#include <Core/TimeManager.h>
#include <Resources/ResourceManager.h>
#include <Generated/LogoGame.rfks.h>

void LogoGame::Start()
{
	isShowing = false;
	showTimer = 0.0f;
	imageLogo = gameObject->GetComponent<UI::Image>();
	imageLogo->opacity = 0;
}
void LogoGame::UpdateScript()
{
	if (!isShowing && Resources::ResourceManager::LoadFinish())
	{
		if (showTimer < showTime)
		{
			showTimer += (float)Core::TimeManager::GetDeltaTime();
		}
		else
		{
			opacityValue = 1;
			isShowing = true;
		}
		imageLogo->opacity = 1;
	}
	else
	{
		if (opacityValue > 0)
		{
			opacityValue -= (float)Core::TimeManager::GetDeltaTime();
			imageLogo->opacity = opacityValue ;
		}
	}
}

std::string LogoGame::SaveScript()
{
	std::string content = std::to_string(showTime) + ',';
	return content;
}

void LogoGame::LoadScript(std::string data)
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


	showTime = std::stof(dataCut[1]);
}