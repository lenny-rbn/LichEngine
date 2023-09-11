#pragma once


#include <Editor/IEditorWindow.h>

namespace SceneManagement 
{
	class Scene;
	class SceneGraph;
	class SceneManager;
}

namespace EditorLich
{
	class FileBrowser;
	enum HierachyAction
	{
		NONE = 0,
		REMOVE = 1,
		ADD = 2
	};

	class HierachyWindow : public IEditorWindowInterraction
	{
	public:
		HierachyWindow(SceneManagement::SceneManager* sceneManager, FileBrowser* filebrowser);
		~HierachyWindow();

		void Update() override;
		void* GetItemSelected(ObjectType& type) override;
		void SetItemSelected(SceneManagement::GameObject* gO);

		void SetCurrentSceneGraph(SceneManagement::Scene** scene);
		void DeselectingObject();
	private:
		void WriteObjectName();
		void WriteObject(Physics::Transform* transform, int i);

		void DragAndDrop(Physics::Transform* transform);

		void RegisterHierachyInput(Physics::Transform* transform);
		void RegisterSwapInput(Vector2D pos);
		void RegisterChildParentInput(Vector2D pos);

		void HierachyActions(Physics::Transform* transform);
		void HierachySwapObject(std::vector<Physics::Transform*>* ptrArray,int nextPos);
		void HierachyChildParentAction(std::vector<Physics::Transform*>* ptrArray,int nextPos);

		void HierachyMenu();
		void DrawGameObjectNamePopup();
		void CreateNewGameObject();
		void ManageSceneState();
		void SceneNameField();
		
		Core::InputManager* input;
		SceneManagement::Scene** m_pScene;
		SceneManagement::SceneGraph* m_pSceneGraph;
		SceneManagement::SceneManager* m_pSceneManager; 
		EditorLich::FileBrowser* m_pFilebrowser;
		bool m_isSceneNameFieldSelected  = false;
		std::filesystem::path m_oldPath;

		EditorUI::WindowFlags m_windowFlags;
		bool m_isOpen;
		bool m_hasDragDrop = false;
		int m_currentIndex = 0;
		bool m_isPlaying;

		// Hierachy popup variable
		std::string m_nameInput;
		std::string m_errorNameInput = "";
		bool m_openFieldPopup =false;
		Vector2D m_mousePos; 
		Vector2D m_endMousePos; 
		Vector2D m_startMousePos; 
		int m_nextPos;
		const char* m_hierachyMenuName = "Hierachy Menu";
		const char* m_gameObjectInputTextName = "GameObject Input Name";
		SceneManagement::GameObject* m_gameObjectSelected;
		
		Vector2D m_prevMousePos;
		Vector2D m_startPos;

		HierachyAction m_childAction = EditorLich::HierachyAction::NONE;
		int m_hierachyMouvement = 0;

		const char* m_windowName = "Hierachy";
		const int m_verticalIndentOfDropMenu = 10;
		const int m_pixelSizeToAddChild = 20;

	};
}