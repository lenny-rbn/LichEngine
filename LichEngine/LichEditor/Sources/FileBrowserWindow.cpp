

#include <Resources/Texture.h>
#include <Resources/Material.h>
#include <Camera/Skybox.h>
#include <Editor/FileBrowserWindow.h>

EditorLich::FileBrowser::FileBrowser(AssetResources* pAssetResources, Resources::ResourceManager* resourcesManager, SceneManagement::SceneManager* sceneManager)
{
	m_pResourcesManager = resourcesManager;
	m_pAssetResouces = pAssetResources;
	m_folderWindowOpen = false;
	m_pSceneManager = sceneManager;
	SearchFileAndDirectory();
}

EditorLich::FileBrowser::~FileBrowser()
{
}

void EditorLich::FileBrowser::Update()
{
	m_windowFlags = EditorUI::WindowOptions::MenuBar;

	if (EditorUI::CreateUI(EditorUI::TypeUI::Window, m_windowName, &m_isOpen, m_windowFlags))
	{
		PopupContextuel();

		if (m_folderWindowOpen)
			EditorUI::CallPopup("Folder");

		StringPopup();

		PathChildWindow();
		ShowFiles();
		ShowDirectories();

		WindowPopupContext();

		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}
	else
	{
		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}


}

void* EditorLich::FileBrowser::GetItemSelected(ObjectType& type)
{
	type = m_type;
	return m_pData;
}

void EditorLich::FileBrowser::RenameFile(const char* path, const char* newName)
{
	std::filesystem::path old = path;

	if (!std::filesystem::exists(old)) return;

	std::string newPath = old.parent_path().string() + "/" + newName + ".lscene";

	std::filesystem::rename(path, newPath);
	SearchFileAndDirectory();

}


void EditorLich::FileBrowser::SearchFileAndDirectory()
{
	m_fileArray.clear();
	m_directoryArray.clear();
	m_currentDirectoryArray.clear();
	m_currentFileArray.clear();

	for (auto& p : std::filesystem::recursive_directory_iterator(m_basePath))
	{
		if (p.is_directory())
		{
			if (p.path().parent_path() == m_currentBasePath)
				m_currentDirectoryArray.push_back(p);

			m_directoryArray.push_back(p);
		}

		if (p.is_regular_file())
		{
			if (p.path().parent_path() == m_currentBasePath)
				m_currentFileArray.push_back(p);

			m_fileArray.push_back(p);
			m_pAssetResouces->AddAssetResouce(p.path());

			if (p.path().extension().string() == ".fbx" || p.path().extension().string() == ".dae")
				m_pResourcesManager->ImportAnimAndSkeletonFiles(p.path().string().c_str());
		}
	}
}

void EditorLich::FileBrowser::ShowFiles()
{
	for (auto iter = m_currentFileArray.begin(); iter != m_currentFileArray.end(); iter++)
	{
		Resources::Texture* text = nullptr;

		std::string fileExtension = iter->path().extension().string().c_str();
		std::string filePath = iter->path().string().c_str();
		std::string fileName = iter->path().filename().string().c_str();

		if (fileExtension == ".jpg" || fileExtension == ".png")
		{
			std::string name = fileName;
			name = name.substr(0, name.find_first_of('.'));
			if (m_pResourcesManager->HasResource<Resources::Texture>(name))
			{
				text = m_pResourcesManager->GetResource<Resources::Texture>(name);
				if (!text->IsCreated())
				{
					text = Resources::ResourceManager::GetResource<Resources::Texture>("default");
				}
			}
			else
			{
				Resources::Create(Resources::ResourceType::TEXTURE, filePath, name);
				m_currentResourcesLoad.push_back(name);
				text = m_pResourcesManager->GetResource<Resources::Texture>("default");
			}
		}
		if (fileExtension == ".obj" || fileExtension == ".fbx" || fileExtension == ".dae")
		{
			text = Resources::ResourceManager::GetResource<Resources::Texture>("MeshIcon");
			if (!text->IsCreated())
			{
				text = Resources::ResourceManager::GetResource<Resources::Texture>("default");
			}
		}

		if (fileExtension == ".lmat")
		{
			text = Resources::ResourceManager::GetResource<Resources::Texture>("MatIcon");
			if (!text->IsCreated())
			{
				text = Resources::ResourceManager::GetResource<Resources::Texture>("default");
			}
		}
		if (fileExtension == ".lscene")
		{
			text = Resources::ResourceManager::GetResource<Resources::Texture>("SceneIcon");
			if (!text->IsCreated())
			{
				text = Resources::ResourceManager::GetResource<Resources::Texture>("default");
			}
		}
		if (fileExtension == ".lskybox")
		{
			text = Resources::ResourceManager::GetResource<Resources::Texture>("SkyIcon");
			if (!text->IsCreated())
			{
				text = Resources::ResourceManager::GetResource<Resources::Texture>("default");
			}
		}



		if (text != nullptr)
		{
			EditorUI::ImageField((EditorUI::TextureID)text->GetTexture(), Vector2D(25, 25));
			EditorUI::SameLine();
		}
		if (EditorUI::SelectableField("", fileName.c_str(), EditorUI::MouseButtonUI::RightButton))
		{
			m_currentEntry = (*iter);
			m_mousePos = EditorUI::GetItemPos();
			m_mousePos.y -= 35.0f;
			EditorUI::CallPopup("MenuContextuel");
		}
		if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton))
		{
			if (EditorUI::IsMouseButtonDoubleClic(EditorUI::MouseButtonUI::LeftButton) && m_pAssetResouces->IsScene(filePath))
			{
				m_currentEntry = (*iter);
				std::string name = fileName;
				size_t offset = name.find_first_of(".");
				name = name.substr(0, offset);
				m_pSceneManager->ChangeScene(name, filePath);
			}

		}
		SelectionMaterialForInspector(&(*iter));
		SelectionSkyboxForInspector(&(*iter));
	}
}

void EditorLich::FileBrowser::ShowDirectories()
{
	for (auto iter = m_currentDirectoryArray.begin(); iter != m_currentDirectoryArray.end(); iter++)
	{
		Resources::Texture* text = Resources::ResourceManager::GetResource<Resources::Texture>("folder");

		if (text == nullptr || !text->IsCreated()) text = m_pResourcesManager->GetResource<Resources::Texture>("default");

		EditorUI::ImageField((EditorUI::TextureID)text->GetTexture(), Vector2D(20, 20));
		EditorUI::SameLine();

		if (EditorUI::SelectableField("", iter->path().filename().string().c_str(), EditorUI::MouseButtonUI::RightButton))
		{
			m_currentEntry = (*iter);
			m_mousePos = EditorUI::GetItemPos();
			m_mousePos.y -= 35.0f;
			EditorUI::CallPopup("MenuContextuel");
		}
		if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton))
		{
			m_currentEntry = (*iter);
			m_currentBasePath = (*iter).path().string();
			SearchFileAndDirectory();
			break;
		}
	}
}

void EditorLich::FileBrowser::PathChildWindow()
{

	if (EditorUI::CreateUI(EditorUI::TypeUI::MenuBar))
	{
		EditorUI::ColorStyle idx = EditorUI::ColorObject::ButtonCol | EditorUI::ColorObject::ButtonHovered;
		EditorUI::VariableStyle var = EditorUI::VariableStyleFlags::IndentSpace;
		EditorUI::PushStyle(var, 1);
		EditorUI::PushStyle(idx, Vector4D(0, 0, 0, 0));

		std::string returnPath = ".." + m_currentBasePath;
		if (m_currentBasePath != m_basePath)
		{
			if (EditorUI::Button(returnPath.c_str()))
			{

				m_currentBasePath = m_currentEntry.path().parent_path().string();
				m_currentEntry.assign(m_currentBasePath);
				for (auto i = m_currentResourcesLoad.begin(); i != m_currentResourcesLoad.end(); i++)
				{
					Resources::ResourceManager::UnloadResource(i->c_str());
				}
				SearchFileAndDirectory();
			}

		}
		else
		{
			EditorUI::WriteText(returnPath.c_str());
		}
	}
	EditorUI::PopStyle(EditorUI::StyleType::Color);
	EditorUI::PopStyle(EditorUI::StyleType::Var);

	EditorUI::End(EditorUI::TypeUI::MenuBar, __FILE__, __LINE__);
}




void EditorLich::FileBrowser::StringPopup()
{
	if (EditorUI::CreatePopup("Folder"))
	{

		EditorUI::SetWindowPos(m_mousePos);
		EditorUI::TextField("Name", m_nameInput, 100);
		EditorUI::SameLine();
		if (EditorUI::Button("Validate"))
		{
			if (m_actionIndex == 0)	
			{
				CreateEngineDirectory();
			}
			if (m_actionIndex == 1)
			{
				RenameDirectory();
			}
			if (m_actionIndex == 2)
			{
				std::string path = m_currentBasePath + "\\" + m_nameInput.c_str() + ".lmat";
				std::string name = m_nameInput + ".lmat";

				Resources::Material* material = new Resources::Material(name.c_str(), path.c_str());
				delete material;

				m_folderWindowOpen = false;
				EditorUI::Close(EditorUI::TypeUI::Popup);
				SearchFileAndDirectory();
				m_nameInput.clear();
			}
			if (m_actionIndex == 3)
			{
				std::string path = m_currentBasePath + "\\" + m_nameInput.c_str() + ".lskybox";
				std::string name = m_nameInput + ".lskybox";

				CameraRender::SkyBox* skybox = new CameraRender::SkyBox(name.c_str(), path.c_str());
				delete skybox;

				m_folderWindowOpen = false;
				EditorUI::Close(EditorUI::TypeUI::Popup);
				SearchFileAndDirectory();
				m_nameInput.clear();
			}


		}
		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}
}

void EditorLich::FileBrowser::PopupContextuel()
{
	if (EditorUI::CreatePopup("MenuContextuel"))
	{
		m_mousePos = EditorUI::GetItemPos();
		m_mousePos.y -= 35.0f;
		if (EditorUI::CreateMenuItem("Create Folder"))
		{
			m_folderWindowOpen = true;
			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 0;
		}
		if (EditorUI::CreateMenuItem("Delete"))
		{
			std::filesystem::remove(m_currentEntry);
			SearchFileAndDirectory();
			ImGui::CloseCurrentPopup();
		}
		if (EditorUI::CreateMenuItem("Rename"))
		{
			m_folderWindowOpen = true;

			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 1;
		}
		ImGui::Separator();
		if (EditorUI::CreateMenuItem("Create Material"))
		{
			m_folderWindowOpen = true;

			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 2;
		}
		if (EditorUI::CreateMenuItem("Create Skybox"))
		{
			m_folderWindowOpen = true;

			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 3;
		}

		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}

}

void EditorLich::FileBrowser::WindowPopupContext()
{
	Vector2D canvas_sz = EditorUI::GetWindowSizeAvailable();
	EditorUI::Button("canvas", canvas_sz, true, (EditorUI::MouseButtonUI)(ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight));
	const bool is_hovered = EditorUI::ItemAction(EditorUI::ActionType::Hovered);
	if (EditorUI::IsMouseButtonRelease(EditorUI::MouseButtonUI::RightButton) && is_hovered)
		EditorUI::CallPopup("WindowContext");


	if (EditorUI::CreatePopup("WindowContext"))
	{
		if (EditorUI::CreateMenuItem("Create Folder"))
		{
			m_folderWindowOpen = true;
			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 0;
		}
		EditorUI::Separator();
		if (EditorUI::CreateMenuItem("Create Material"))
		{
			m_folderWindowOpen = true;

			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 2;
		}
		if (EditorUI::CreateMenuItem("Create Skybox"))
		{
			m_folderWindowOpen = true;

			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_actionIndex = 3;
		}

		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}
}

void EditorLich::FileBrowser::OpenDirectory(std::filesystem::directory_entry folder)
{
	if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton))
	{
		m_currentEntry = folder;
		m_currentBasePath = folder.path().string();
		SearchFileAndDirectory();

	}
}

void EditorLich::FileBrowser::CreateEngineDirectory()
{
	if (m_nameInput == "")
	{
		EditorUI::Close(EditorUI::TypeUI::Popup);
		return;
	}

	std::string allPath = m_currentBasePath + "\\" + m_nameInput.c_str();
	if (std::filesystem::create_directory(allPath))
	{
		SearchFileAndDirectory();
		m_folderWindowOpen = false;
		EditorUI::Close(EditorUI::TypeUI::Popup);
		m_nameInput.clear();
	}
}

void EditorLich::FileBrowser::RenameDirectory()
{
	if (m_nameInput == "")
	{
		EditorUI::Close(EditorUI::TypeUI::Popup);
		return;
	}
	std::string allPath = m_currentEntry.path().parent_path().string() + "\\" + m_nameInput.c_str();
	if (m_currentEntry.is_regular_file())
	{
		allPath += m_currentEntry.path().extension().string();
	}
	std::filesystem::rename(m_currentEntry.path(), allPath);
	SearchFileAndDirectory();
	m_folderWindowOpen = false;
	EditorUI::Close(EditorUI::TypeUI::Popup);
	m_nameInput.clear();
}

std::string remove_extension(const std::string& filename)
{
	size_t extension = filename.find_last_of(".");
	if (extension == std::string::npos)
		return filename;

	return filename.substr(0, extension);
}

void EditorLich::FileBrowser::SelectionMaterialForInspector(std::filesystem::directory_entry* directories)
{
	if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton))
	{
		if (m_pAssetResouces->IsMaterial(directories->path()))
		{
			m_hasItemSelected = true;
			std::string name = remove_extension(directories->path().filename().string());

			if (!m_pResourcesManager->HasResource<Resources::Material>(name))
				m_pResourcesManager->CreateSpecificResource(Resources::MATERIAL, directories->path().string().c_str(), name.c_str());

			m_pData = (void*)m_pResourcesManager->GetResource<Resources::Material>(name);
			m_type = ObjectType::MATERIAL;
		}
		else
		{
			m_hasItemSelected = true;
			m_pData = nullptr;
			m_type = ObjectType::NONE;
		}
	}
}

void EditorLich::FileBrowser::SelectionSkyboxForInspector(std::filesystem::directory_entry* directories)
{

	if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton))
	{
		if (m_pAssetResouces->isSkybox(directories->path()))
		{
			m_hasItemSelected = true;
			std::string name = remove_extension(directories->path().filename().string());

			if (!m_pResourcesManager->HasResource<CameraRender::SkyBox>(name))
				m_pResourcesManager->CreateSpecificResource(Resources::SKYBOX, directories->path().string().c_str(), name.c_str());

			m_pData = (void*)m_pResourcesManager->GetResource<CameraRender::SkyBox>(name);
			m_type = ObjectType::SKYBOX;
		}
		else if (!m_hasItemSelected)
		{
			m_hasItemSelected = true;
			m_pData = nullptr;
			m_type = ObjectType::NONE;
		}
	}
}
