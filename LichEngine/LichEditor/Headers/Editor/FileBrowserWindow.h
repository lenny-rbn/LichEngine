#pragma once
#include <vector>
#include <maths.h>
#include <ToolBox.h>
#include <Editor/AssetsResources.h>
#include <Editor/IEditorWindow.h>
#include <Scene/SceneManager.h>


namespace EditorLich
{
	class FileBrowser : public IEditorWindowInterraction
	{
	public:
		FileBrowser(AssetResources* pAssetResources,Resources::ResourceManager* resourcesManager, SceneManagement::SceneManager* m_pSceneManager);
		~FileBrowser();
		void Update() override;
		void* GetItemSelected(ObjectType& type) override;
		void RenameFile(const char* path, const char* newName);

	private:

		void SearchFileAndDirectory();
		void ShowFiles();
		void ShowDirectories();
		void PathChildWindow();
		void StringPopup();
		void PopupContextuel();
		void WindowPopupContext();
		void OpenDirectory(std::filesystem::directory_entry folder);
		void CreateEngineDirectory();
		void RenameDirectory();
		void SelectionMaterialForInspector(std::filesystem::directory_entry* directories);
		void SelectionSkyboxForInspector(std::filesystem::directory_entry* directories);


		bool m_isOpen = true;
		bool m_folderWindowOpen = false;
		int m_actionIndex;
		std::string m_nameInput;
		Vector2D m_mousePos;
		std::string m_currentBasePath = ".\\Resources";
		const std::string m_basePath = ".\\Resources";

		const char* m_windowName = "Projets Assets";

		std::filesystem::directory_entry m_currentEntry;
		std::filesystem::directory_entry m_selectedEntry;
		std::vector<std::filesystem::directory_entry> m_directoryArray;
		std::vector<std::filesystem::directory_entry> m_fileArray;
		std::vector<std::filesystem::directory_entry> m_currentDirectoryArray;
		std::vector<std::filesystem::directory_entry> m_currentFileArray;
		EditorUI::WindowFlags m_windowFlags = 0;
		AssetResources* m_pAssetResouces;
		Resources::ResourceManager* m_pResourcesManager;
		SceneManagement::SceneManager* m_pSceneManager;
		std::vector<std::string> m_currentResourcesLoad;
	};


}