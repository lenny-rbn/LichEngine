#include <pch.h>
#include <sstream>
#include <Debugs/Log.h>
#include <Core/RHI.h>
#include <Debugs/Log.h>
#include <Renderer/Mesh.h>
#include <Scene/GameObject.h>
#include <Camera/Camera.h>
#include <Animation/Animator.h>
#include <Refureku/Refureku.h>
#include <Physics/Transform.h>
#include <ToolBox.h>

#include <Generated/App.rfks.h>
#include <Resources/SoundInstance.h>
#include <Resources/ResourceManager.h>


namespace Core
{
	App::App()
	{
		m_pResourcesManager = new Resources::ResourceManager();
		m_pWindow = new Window();
		LoadApplicationSettings("AppSettings.txt");
		m_pWindow->InitWindow(currentSettings);
		WrapperRHI::RHI::InitOpenGL();
	}

	App::~App() {}

	void App::Initialize()
	{
		m_pInputManager = new InputManager(m_pWindow->GetWindow());
		m_pInputManager->Init();
		m_pTimeManager = new TimeManager();
		m_pAudioManager = new SoundSystem::AudioManager();
		m_pResourcesManager = new Resources::ResourceManager();


		CreateResources();
		m_pResourcesManager->CreateResourceMonothread<Resources::SoundInstance>("Resources/Sounds/Launch.waw","launch");

		m_pSceneManager = new SceneManagement::SceneManager((int)m_pWindow->GetWindowSize().x, (int)m_pWindow->GetWindowSize().y, m_pInputManager,this);
		
		SceneManagement::Scene* pCurrentScene = m_pSceneManager->GetCurrentScene();

		SceneManagement::GameObject* camera = m_pSceneManager->GetCurrentScene()->GetCameraTransform()->GetGameObject();
		Vector3D pos = camera->GetTransform()->GetGlobalPosition();
		Vector3D forward = camera->GetTransform()->GetVectorForward();
		m_pAudioManager->engine->setListenerPosition({ pos.x,pos.y,pos.z }, { forward.x,forward.y,forward.z });
		
		m_pSceneManager->SetCamera(pCurrentScene->GetCamera());
		
		Resources::ResourceManager::GetThreadPool()->EnableThreads();
	}

	void App::Update()
	{
		WindowAPI::PollEvents();
		m_pTimeManager->Update();
		m_pInputManager->Update();
		m_pSceneManager->Update();
	}

	void Core::App::Draw()
	{
		m_pSceneManager->Draw();
	}

	void App::Close()
	{
		WriteApplicationSettings();
		m_pInputManager->Destroy();
		m_pSceneManager->Destroy();
		m_pWindow->Destroy();
		m_pResourcesManager->Clear();

		delete m_pWindow;
		delete m_pInputManager;
		delete m_pTimeManager;
		delete m_pResourcesManager;
		delete m_pSceneManager;
	}

	void App::LoadApplicationSettings(std::string path)
	{
		std::string content;
		Utils::ReadFile(path, content);

		std::string line;
		std::istringstream iss(content);

		if (std::filesystem::exists(path))
		{

			std::getline(iss, currentSettings.name);
			std::getline(iss, line);
			currentSettings.width = std::stoi(line);
			std::getline(iss, line);
			currentSettings.height = std::stoi(line);
		}
		else
		{
			currentSettings.name = "LichEngine";
			currentSettings.width = 1920;
			currentSettings.width = 1080;
		}
	}

	void App::WriteApplicationSettings()
	{
		std::string data = currentSettings.name;
		data += "\n" + std::to_string(currentSettings.width);
		data += "\n" + std::to_string(currentSettings.height);

		Utils::WriteFile("AppSettings.txt", data);
	}

	void App::Run()
	{
		while (!WindowAPI::WindowShouldClose(m_pWindow->GetWindow()) && !isQuiting)
		{
			Update();
			Draw();
			WindowAPI::SwipBuffers(m_pWindow->GetWindow());
		}
	}

	void App::CreateResources()
	{
		m_pResourcesManager->InitAnimAndSkeletonFiles("SaveAnimSkelFiles.txt");

		Resources::Create(Resources::ResourceType::TEXTURE, "Resources/Textures/default.png", "default", Resources::ResourceLayer::MAIN);
		Resources::Create(Resources::ResourceType::MODEL, "Resources/Models/Skybox/CubeSky.obj", "CubeSky", Resources::ResourceLayer::MAIN);
		Resources::Create(Resources::ResourceType::MATERIAL, "Resources/Materials/defaultMat.lmat", "defaultMat", Resources::ResourceLayer::MAIN);

		//Shaders
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/geometry.ls", "geometryShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/ssao.ls", "ssaoShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/blur.ls", "blurShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/lighting.ls", "lightingShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/hdr.ls", "hdrShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/gaussianBlur.ls", "gaussianBlurShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/postprocess.ls", "postprocessShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/shadowDirLight.ls", "shadowDirLightShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/shadowPointLight.ls", "shadowPointLightShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/PBR.ls", "PBRShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/PBRForward.ls", "PBRForwardShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/skybox.ls", "skyboxShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/UIText.ls", "UITextShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/UIImage.ls", "UIImageShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/picking.ls", "PickingShader");
		m_pResourcesManager->CreateResourceMonothread<Resources::Shader>("Resources/Shaders/gizmo.ls", "GizmoShader");

	}
}
