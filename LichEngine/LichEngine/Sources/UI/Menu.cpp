
#include <pch.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include <Core/TimeManager.h>
#include <Generated/Menu.rfks.h>

MenuGame::MenuGame()
{

}

void MenuGame::Start()
{
	m_pTextComponent = gameObject->GetComponent<UI::Text>();
	m_pTextComponent->SetOpacity(0);
		m_pSoundEvent = gameObject->GetComponent<SoundSystem::SoundEvent>();
		m_pSoundEvent->PlaySound(0);
	
}

void MenuGame::UpdateScript()
{
	if (Resources::ResourceManager::LoadFinish())
	{
		if (!isHide)
		{
			if (!fadeOut)
			{
				opacityValue -= (float)Core::TimeManager::GetDeltaTime();
				if (opacityValue < 0)
				{
					fadeOut = !fadeOut;
					opacityValue = 0;
				}
			}
			else
			{
				opacityValue += (float)Core::TimeManager::GetDeltaTime();
				if (opacityValue > 1)
				{
					fadeOut = !fadeOut;
					opacityValue = 1;
				}
			}
			m_pTextComponent->SetOpacity(opacityValue);

			if (input->IsPressed("Enter"))
			{
				m_pSoundEvent->StopSound(0);
				gameObject->GetSceneGraph()->GetScene()->GetSceneManager()->ChangeSceneInGame("Level 1.lscene", "Resources\\Level 1.lscene", false);
			}
		}
		else
		{
			m_pTextComponent->SetOpacity(0);
			if (fadeTimer < fadeTime)
			{
				fadeTimer += (float)Core::TimeManager::GetDeltaTime();
			}
			else
			{
				isHide = false;
			}
		}
	}
	if (input->IsPressed("Escape"))
	{
		gameObject->GetSceneGraph()->GetScene()->GetSceneManager()->QuitApp();
	}

}




void MenuGame::OnCollisionEnter(SceneManagement::GameObject* other)
{
	LDEBUG(gameObject->GetName() + " collided with " + other->GetName());

}

void MenuGame::OnCollisionStay(SceneManagement::GameObject* other)
{

}

void MenuGame::OnCollisionExit(SceneManagement::GameObject* other)
{
	LDEBUG("Collision between " + gameObject->GetName() + " and " + other->GetName() + " stopped");
}

void MenuGame::OnTriggerEnter(SceneManagement::GameObject* other)
{
	LDEBUG(gameObject->GetName() + " has triggered " + other->GetName());
}

void MenuGame::OnTriggerExit(SceneManagement::GameObject* other)
{

}
#pragma endregion

std::string MenuGame::SaveScript()
{
	std::string content = std::to_string(fadeTime) + ',';
	return content;
}

void MenuGame::LoadScript(std::string data)
{
	const int sizeData = 2;
	std::string dataCut[sizeData];
	size_t index = 0,baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',',baseIndex + 1);
		dataCut[i] = data.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}
	fadeTime = std::stof(dataCut[1]);

}