#include <pch.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include <Generated/PauseMenu.rfks.h>

void PauseMenu::Start()
{

}


void PauseMenu::UpdateScript()
{
	if (input->IsPressed("Enter"))
	{
		gameObject->GetSceneGraph()->GetScene()->GetSceneManager()->QuitApp();
	}
}

std::string PauseMenu::SaveScript()
{
	return ",";
}

void PauseMenu::LoadScript(std::string data)
{
	
}