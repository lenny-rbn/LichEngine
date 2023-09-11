#pragma once

#include <Scene/SceneGraph.h>
#include <Scene/GameObject.h>

#include <Editor/IEditorWindow.h>
#include <Editor/AssetsResources.h>
#include <Animation/Animator.h>
#include <Refureku/Refureku.h>
#include <Resources/Scripting.h>


// Function to predicate if a field is valid to be use
bool IsFieldValid(const rfk::Field&, void*);

namespace EditorLich
{

	struct FieldInfo
	{
		bool changeField = false;
		bool asBeenHandle = false;
		rfk::EEntityKind type;
		std::string varName;
		std::string nameClass;
		const rfk::Archetype* archetype;
	};

	class InspectorWindow : public EditorLich::IEditorWindow
	{
	public:
		
		InspectorWindow(AssetResources* assetResource);
		~InspectorWindow();
		
		void Update() override;

		void AddInteractableWindow(IEditorWindowInterraction* window);

	private:

		bool m_isOpen = true;
		std::vector<IEditorWindowInterraction*> m_interactableWindows;
		EditorUI::WindowFlags m_windowFlags = 0;
		EditorUI::Tree_Flags m_nodeFlags = EditorUI::TreeOptions::OpenArrow | EditorUI::TreeOptions::OpenDoubleClick | EditorUI::TreeOptions::SpanAvailWidth | EditorUI::TreeOptions::OpenAtStart | EditorUI::TreeOptions::TreeFramed;
		
		void* m_pObjectData;
		ObjectType m_objectType;
		Physics::Transform* m_pTransform = nullptr;
		SceneManagement::GameObject* m_pObjSelected = nullptr;
		Resources::Material* m_pMatSelected = nullptr;
		CameraRender::SkyBox* m_pSkyboxSelected = nullptr;

		const char* m_windowName = "Inspector";

		AssetResources* m_pAssetResources;
		Resources::Resource** m_pSelected;
		SceneManagement::GameObject** m_pObjFieldSelected;
		Resources::ResourceType m_typeSelected;
		
		void CheckItemSelected();
		void ConvertItemSelectedIntoData();
		
		void DrawMaterial();
		void DrawSkybox();
		void DrawGameObject();
		void GameObjectName();

		void DrawComponents();
		void DrawFields(rfk::Vector < const rfk::Field* > fields, void* data, bool& asChange);
		void DrawArray(const rfk::Field* field, void* data, bool& asChange, const char* nameVar);
		void DrawFieldFundamental(const rfk::Field* field, void* data, bool& asChange, bool& isHandle);
		void DrawPrimitive(std::string type, std::string name, void* data, bool& asChange, const rfk::Archetype* archetype, const rfk::Field* field, bool& isHandle);
		void DrawEnum(const rfk::Field* field,void* data,FieldInfo info);
		void DrawClassField(const rfk::Field* field, void* data, FieldInfo info);
		rfk::Vector<const rfk::Field*> GetFields(void* data,const rfk::Class* classIntance);
		void DrawField(const rfk::Field* field,bool& asBeenModified,void* dataInstance);

		void CleanVariableName(std::string& name);

		bool IsArrayField(const rfk::Field* field);
		
		bool ComponentCheckbox(bool* ptrState, int seed);

		int m_indexComponentAdd;
		SceneManagement::ComponentType m_typeComponentSelected;
		const char* m_nameComponentPopup = "Component Options";
		const char* m_nameResourcesPopup = "Resources";

		void DrawComponent(const rfk::Class* classInstance, SceneManagement::Component* instance);

		FieldInfo GetFieldInfo(const rfk::Field* field);

		bool ResourcePopup();
		void ComponentPopup();

		void ChangeScriptingClass(Resources::Scripting* script,std::vector<SceneManagement::Component*>* components, int index);
		bool IsAlreadySet(std::string currentScriptName , std::string inputScriptName);
		bool IsWrongScriptName(const rfk::Class* scriptClass);
		bool m_wrongScriptingName= false;
		const char* m_errorMsg[2] = { "Name invalid", "Is already setup" };
		unsigned int m_errorIndex = 0;

		template<class T>
		void DrawComponent(SceneManagement::Component* instance)
		{
			if (EditorUI::ItemAction(EditorUI::ActionType::Clicked,EditorUI::MouseButtonUI::RightButton))
			{
				EditorUI::CallPopup("ComponentOptions");
			}
			bool asChange = false;
			asChange = ResourcePopup();
			ComponentPopup();
			
			const rfk::Archetype* archetype = rfk::getArchetype<T>();
			const rfk::Class* classInstance = rfk::classCast(archetype);
			rfk::Vector<const rfk::Field*> fields = GetFields((void*)instance,classInstance);


			DrawFields(fields,(void*)instance,asChange);

			if (!asChange) return;
			instance->ApplyEditorChange();

		}

	};

}