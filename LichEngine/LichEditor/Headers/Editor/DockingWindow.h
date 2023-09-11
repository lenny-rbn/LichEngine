#pragma once
#include <vector>
#include <Editor/IEditorWindow.h>
#include <EditorLich/WindowSubscriber.h>
#include <Editor/AssetsResources.h>
#include <Editor/EngineData.h> 
#include <Scene/SceneManager.h> 

namespace Core
{
	class App;
}

namespace EditorLich
{

	class DockingWindow : public Core::Editor::WindowSubscriber
	{
	public:
		DockingWindow(EngineData* engineData, Core::App* app);
		~DockingWindow();

		void Update() ;
		void AddWindow(IEditorWindow* window);
		void SetWindowSize(const Vector2D size) override;
	private:

		void CreateEngineWindow(EngineData* engineData,Core::App* app);
		void UpdateWindowSize();
		void EngineMenuBar();
		void HandleDockingSpace();
		void UpdateEngineWindow();

		bool m_isOpen;
		EditorUI::WindowFlags m_window_flags;
		
		std::vector<IEditorWindow*> m_editorWindow;
		
		Core::Window * m_pWindow;
		Vector2D m_currentSize;
		AssetResources m_assetResources;

		const char* m_windowName = "DockSpace Window";
		const char* m_fileMenuName = "File";
		SceneManagement::SceneManager* m_pSceneManager;
	};

}