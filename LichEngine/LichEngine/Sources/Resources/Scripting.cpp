#include <pch.h>
#include <Scene/GameObject.h>
#include <Generated/Scripting.rfks.h>
#include "..\..\Headers\Resources\Scripting.h"


namespace Resources
{
	Scripting::Scripting()
	{
		componentType = SceneManagement::ComponentType::SCRIPTING;
	}

	Scripting::Scripting(SceneManagement::GameObject* gO)
	{
		SetGameObject(gO);
		input = p_pGameObject->GetInputManager();
		transform = p_pGameObject->GetTransform();
		componentType = SceneManagement::ComponentType::SCRIPTING;
	}

	void Scripting::SetGameObject(SceneManagement::GameObject* gO)
	{
		gameObject = gO;
		p_pGameObject = gO;
	}

	std::string Scripting::SaveData()
	{
		std::string content = "~8,";
		content += currentName + ',';
		content += SaveScript();
		return content;
	}

	void Scripting::ApplyEditorChange()
	{

	}

	void Scripting::LoadData(std::string data)
	{
		std::string test = data;
		const int sizeData = 1;
		std::string dataCut[sizeData];
		size_t index = 0;
		size_t baseIndex = 0;
		for (size_t i = 0; i < sizeData; i++)
		{
			index = data.find(',',baseIndex + 1);
			dataCut[i] = data.substr(baseIndex,index - baseIndex);
			baseIndex = index + 1;
		}
		ConvertScript(this,dataCut[0].c_str(),data);		
	}

	void Scripting::Update(Physics::Transform& transform)
	{
		if (transform.IsPlaying())
		{
			if (!started)
			{
				Start();
				started = true;
			}

			UpdateScript();
		}
	}

	void ConvertScript(Scripting* script,const char* name,std::string data)
	{


		SceneManagement::GameObject* go = script->gameObject;
		Core::InputManager* inputManager = script->input;
		Physics::Transform* transform = script->transform;

		const rfk::Class* scriptClass = rfk::getDatabase().getFileLevelClassByName(name);
		if (scriptClass == nullptr)
		{
			LDEBUG("This script doesn't exist");
			return;
		}
		rfk::UniquePtr<Resources::Scripting> ptr = scriptClass->makeUniqueInstance<Resources::Scripting>();
	
		script = (Resources::Scripting*)ptr.release();
		script->currentName = name;
		script->nameInput = name;
		script->gameObject = go;
		script->input = inputManager;
		script->transform = transform;
		script->LoadScript(data); 

		std::vector<SceneManagement::Component*>* components = script->gameObject->GetComponents();
		int index = script->gameObject->GetComponentIndex(SceneManagement::ComponentType::SCRIPTING);
		(*components)[index] = script;
	}
}