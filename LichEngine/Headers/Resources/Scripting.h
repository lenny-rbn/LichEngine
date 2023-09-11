#pragma once
#include <Scene/Component.h>
#include <Scene/GameObject.h>
#include <Core/InputManager.h>
#include <Generated/Scripting.rfkh.h>


namespace Resources NAMESPACE()
{
	class CLASS() LICH_ENGINE Scripting : public SceneManagement::Component
	{
	public:
		Scripting();
		Scripting(SceneManagement::GameObject * gO);
		~Scripting() {};

		//Set gameobject

		bool started = false;

		std::string nameInput = "Scripting";
		std::string currentName = "Scripting";

		Core::InputManager* input = nullptr;
		SceneManagement::GameObject* gameObject = nullptr;
		Physics::Transform* transform = nullptr;


		void SetGameObject(SceneManagement::GameObject* gO);

		std::string SaveData() override;
		void ApplyEditorChange() override;
		void LoadData(std::string data) override;
		void Update(Physics::Transform & transform) override;

		virtual void Start() {};
		virtual void UpdateScript() {};
		virtual void FixedUpdateScript() {};
		virtual void OnTriggerEnter(SceneManagement::GameObject* other) {};
		virtual void OnTriggerExit(SceneManagement::GameObject* other) {};
		virtual void OnCollisionStay(SceneManagement::GameObject* other) {};
		virtual void OnCollisionExit(SceneManagement::GameObject* other) {};
		virtual void OnCollisionEnter(SceneManagement::GameObject* other) {};

		virtual bool Started() { return 0; }
		virtual void LoadScript(std::string data) {};
		virtual std::string SaveScript() { std::string x; return x; };

		Resources_Scripting_GENERATED
	};

	void ConvertScript(Scripting * script,const char* name,std::string data);

};
File_Scripting_GENERATED