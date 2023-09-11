#include <pch.h> 
#include <Debugs/Log.h>
#include <Core/TimeManager.h>
#include <Physics/Collider.h>
#include <Physics/RigidBody.h>
#include <Core/App.h>
#include <Resources/ResourceManager.h>
#include <Generated/SceneManager.rfks.h>


namespace SceneManagement
{
	SceneManager::SceneManager()
	{
		m_pCurrentScene = nullptr;
		m_pRenderManager = new Renderer::RenderManager();

		m_pCurrentScene = nullptr;
		LoadSceneManagerData();


		LoadCurrentScene();
		sceneToLoadInfo.isCall = false;
		m_pRenderManager->Init(1920, 1080);
	}

	SceneManager::SceneManager(int screenWidth, int screenHeight, Core::InputManager* inputManager,  Core::App* app)
	{
		m_pCurrentScene = nullptr;
		m_pInputManager = inputManager;
		m_pRenderManager = new Renderer::RenderManager();

		m_pApp = app;
		m_pCurrentScene = nullptr;
		LoadSceneManagerData();

		m_lastSceneOpenName = m_pCurrentScene->GetName();
		m_lastSceneOpenPath = m_pCurrentScene->GetPath().string();
		m_pRenderManager->Init(screenWidth, screenHeight);
		sceneToLoadInfo.isCall = false;
	}

	SceneManager::~SceneManager()
	{
		SaveSceneManagerData();
		m_pCurrentScene->Unload();
		Resources::ResourceManager::Clear();

		delete m_pCurrentScene;
	}

	Scene* SceneManager::GetCurrentScene()
	{
		return m_pCurrentScene;
	}

	Scene** SceneManager::GetCurrentScenePtr()
	{
		return &m_pCurrentScene;
	}


	bool SceneManager::ChangeScene(std::string name, std::string path)
	{
		Resources::ResourceManager::ClearSceneByLayer(Resources::ResourceLayer::SCENE);
		m_pCurrentScene->Unload();
		delete m_pCurrentScene;

		if (!std::filesystem::exists(path))  return false;
		CreateScene(name, path, false);

		m_lastSceneOpenName = m_pCurrentScene->GetName();
		m_lastSceneOpenPath = m_pCurrentScene->GetPath().string();
		return true;
	}

	bool SceneManager::ChangeScene(const char* name, const char* path, bool isPlay)
	{
		Resources::ResourceManager::ClearSceneByLayer(Resources::ResourceLayer::SCENE);
		m_pCurrentScene->Unload();
		delete m_pCurrentScene;

		if (!std::filesystem::exists(path))  return false;
		CreateScene(name, path, false);
		m_pCurrentScene->ChangePlayingState(isPlay);
		if (!isPlay)
		{
			m_lastSceneOpenName = m_pCurrentScene->GetName();
			m_lastSceneOpenPath = m_pCurrentScene->GetPath().string();
		}
		return true;
	}

	void SceneManager::ChangeSceneInGame(const char* name, const char* path,bool reset)
	{
		sceneToLoadInfo.isCall = true;
		sceneToLoadInfo.name = name;
		sceneToLoadInfo.path = path;
		sceneToLoadInfo.reset = reset;
	}

	bool SceneManager::CreateNewScene(const char* name, const char* path)
	{
		Resources::ResourceManager::ClearSceneByLayer(Resources::ResourceLayer::SCENE);	
		m_pCurrentScene->Unload();
		delete m_pCurrentScene;

		CreateScene(name, path, true);

		m_lastSceneOpenName = m_pCurrentScene->GetName();
		m_lastSceneOpenPath = m_pCurrentScene->GetPath().string();
		return true;
	}

	void SceneManager::ReloadCurrentScene()
	{
		m_pCurrentScene->Unload();
		delete m_pCurrentScene;

		if (!std::filesystem::exists(m_lastSceneOpenPath))  return;
		CreateScene(m_lastSceneOpenName, m_lastSceneOpenPath, false);
	}


	void SceneManager::CreateScene(std::string name, std::string path, bool isNew)
	{
		if (name == "")
		{
			name = "DefaultScene";
			path = "Resources/DefaultScene.lscene";
		}

		Scene* sceneCreate = new Scene(name, path, isNew, m_pInputManager, this);
		m_pCurrentScene = sceneCreate;
		m_pCurrentScene->SetInputManager(m_pInputManager);
		sceneCreate = nullptr;
	}

	void SceneManager::LoadSceneManagerData()
	{
		bool isEmpty = true;
		size_t index = 0;
		size_t baseIndex = 0;
		std::string fileData;
		std::string sceneName;
		std::string scenePath;
		std::ifstream myfile;
		myfile.open("SceneManager.txt");
		LINFO("Scene load:");
		while (std::getline(myfile, fileData))
		{
			isEmpty = false;
			index = fileData.find(',', baseIndex + 1);
			sceneName = fileData.substr(baseIndex, index - baseIndex);
			scenePath = fileData.substr(index - baseIndex + 1, fileData.size());
			bool isNew = std::filesystem::exists(scenePath);
			CreateScene(sceneName, scenePath, !isNew);
		}
		myfile.close();
		if (isEmpty) CreateScene("", "", true);
	}

	void SceneManager::SaveSceneManagerData()
	{
		std::string dataToSave = m_lastSceneOpenName + ',' + m_lastSceneOpenPath + "\n";

		std::ofstream myfile;
		myfile.open("SceneManager.txt");
		myfile << dataToSave;
		myfile.close();
	}

	void SceneManager::Update()
	{
		if (m_pCurrentScene == nullptr)
		{
			LWARNING("You don't have any scene create");
			return;
		}

		m_pCurrentScene->Update();

		if (sceneToLoadInfo.isCall)
		{
			sceneToLoadInfo.isCall = false;
			if(!sceneToLoadInfo.reset)	ChangeScene(sceneToLoadInfo.name, sceneToLoadInfo.path,true);
			if (sceneToLoadInfo.reset)	
			{
				ReloadCurrentScene();
#ifdef LICH_EDITOR
			m_pCurrentScene->ChangePlayingState();
#endif // LICH_EDITOR
			}
		}
	}

	void SceneManager::SaveCurrentScene()
	{
		if (!m_pCurrentScene->IsPlaying())
		{
			m_pCurrentScene->Save();
			return;
		}

		LERROR("You can save during play time");
	}

	void SceneManager::Draw()
	{
		m_pRenderManager->Draw(m_pCurrentScene);
	}

	void SceneManager::Destroy()
	{
		m_pRenderManager->Destroy();
		delete m_pRenderManager;
	}

	void SceneManager::LoadCurrentScene()
	{
		m_pCurrentScene->SetInputManager(m_pInputManager);
		m_pCurrentScene->Load();
		m_pCurrentScene->SetCamera();
	}

	GameObject* SceneManager::GetGameObjectFromID(Vector4D id)
	{
		if (id == Vector4D(0, 0, 0, 0))
			return nullptr;

		std::map<std::string, GameObject*> gameObjects = m_pCurrentScene->GetSceneGraph()->GetSceneNodes();

		for (std::map<std::string, GameObject*>::iterator index = gameObjects.begin(); index != gameObjects.end(); index++)
		{
			if (Vector3D(index->second->GetId()) == Vector3D(id))
				return index->second;
		}

		return nullptr;
	}

	Vector4D SceneManager::GetIDFromPicking(Vector2D viewportCoord, Vector2D viewportPos, Vector2D viewportSize)
	{
		Vector4D id = Vector4D(0, 0, 0, 0);
		Vector2D windowSize = m_pRenderManager->GetWindowSize();
		Vector2D finalCoord = Vector2D((viewportCoord.x - (viewportPos.x + 15)) * windowSize.x / viewportSize.x, (viewportCoord.y - (viewportPos.y + 75)) * windowSize.y / viewportSize.y);

		m_pRenderManager->DrawForPicking(GetCurrentScene());
		WrapperRHI::RHI::ReadPixelColor(finalCoord.x, finalCoord.y, id);
		WrapperRHI::RHI::SetActiveFramebuffer(0);

		return id;
	}

	int SceneManager::GetFramerate()
	{
		return (int)(1 / Core::TimeManager::GetDeltaTime());
	}

	void SceneManager::ChangeCurrentScenePlayingState()
	{
		if (m_pCurrentScene->IsPlaying())
		{
			ReloadCurrentScene();
			WindowAPI::SetInputMode(m_pInputManager->GetWindow(), Input::CURSOR, Mode::NORMAL);
		}
		else
		{
			m_pCurrentScene->ChangePlayingState();
			WindowAPI::SetInputMode(m_pInputManager->GetWindow(), Input::CURSOR, Mode::DISABLED);
		}
	}

	void SceneManager::ChangeSceneName(Scene* scene)
	{
		m_lastSceneOpenName = scene->GetName();
		m_lastSceneOpenPath = scene->GetPath().string();
	}
	
	void SceneManager::QuitApp()
	{
		m_pApp->QuitApplication();
	}
}