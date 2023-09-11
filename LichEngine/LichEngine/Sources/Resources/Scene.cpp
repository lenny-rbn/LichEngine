#include <pch.h>
#include <sstream>

#include <Core/App.h>
#include <Scene/Scene.h>
#include <Core/DirLight.h>
#include <Core/SpotLight.h>
#include <Animation/Animator.h>
#include <Physics/PhysicsManager.h>
#include <Resources/ResourceManager.h>
#include <UI/Text.h>
#include <UI/Image.h>
#include <Sound/SoundEvent.h>
#include <unordered_map>



namespace SceneManagement
{
	Scene::Scene()
	{
#ifdef LICH_EDITOR
		m_isPlaying = false;
#endif // LICH_EDITOR

		m_name = "New Scene";
		m_pPhysicsManager = new Physics::PhysicsManager();
		m_pPhysicsManager->Init(this);
		m_pSceneGraph = new SceneGraph(m_pPhysicsManager, this);
		m_pLightManager = new LightSystem::LightManager();
		m_pGizmos = new Gizmos();

		GameObject* goText = m_pSceneGraph->AddGameObject("Cube");
		GameObject* cameraGO = m_pSceneGraph->AddGameObject("Camera");
		GameObject* hammer = m_pSceneGraph->AddGameObject("Hammer");
		GameObject* floor = m_pSceneGraph->AddGameObject("Wall");
		GameObject* wall = m_pSceneGraph->AddGameObject("Floor");

		cameraGO->AddComponent<CameraRender::Camera>();
		m_pMainCam = cameraGO->GetComponent<CameraRender::Camera>();
		m_pCamTranform = cameraGO->GetTransform();

		goText->GetTransform()->SetLocalPosition(Vector3D(0, 0, 10.f));
	}

	Scene::Scene(std::string name, std::string path, bool isNew, Core::InputManager* inputManager, SceneManager* sceneManager)
	{

#ifdef LICH_EDITOR
		m_isPlaying = false;
#endif // LICH_EDITOR

		m_name = name;
		m_path = path;
		m_pInputManager = inputManager;
		// Create Scene Graph
		m_pPhysicsManager = new Physics::PhysicsManager();
		m_pLightManager = new LightSystem::LightManager();
		m_pPhysicsManager->Init(this);
		m_pSceneGraph = new SceneGraph(m_pPhysicsManager, this);
		m_pMainCam = nullptr;
		m_pGizmos = new Gizmos();
		m_pInputManager = inputManager;
		m_pSceneManager = sceneManager;
		if (!isNew)
		{
			Load();
			SetCamera();
		}
		else
		{
			SceneManagement::GameObject* go = m_pSceneGraph->AddGameObject("Camera");
			go->AddSpecificComponent(SceneManagement::ComponentType::CAMERA);
			go = m_pSceneGraph->AddGameObject("Directional Light");
			go->AddSpecificComponent(SceneManagement::ComponentType::DIR_LIGHT);
			SetCamera();
		}
	}


	void Scene::CreateMesh(std::string name, Vector3D position, Vector3D rotation, Vector3D scale)
	{
		GameObject* newGO = m_pSceneGraph->AddGameObject(name.c_str());
		newGO->AddComponent<Renderer::Mesh>();
		newGO->GetTransform()->SetLocalPosition(position);
		newGO->GetTransform()->SetLocalScale(scale);
		newGO->GetTransform()->SetLocalRotation(rotation);
	}

	GameObject* Scene::CreateMesh(std::string name, Vector3D position, Vector3D scale)
	{
		GameObject* newGO = m_pSceneGraph->AddGameObject(name.c_str());
		newGO->AddComponent<Renderer::Mesh>();
		newGO->GetTransform()->SetLocalPosition(position);
		newGO->GetTransform()->SetLocalScale(scale);
		newGO->GetTransform()->CalculateTemp();
		return newGO;
	}

	Scene::~Scene()
	{
		m_pMainCam = nullptr;
		m_pCamTranform = nullptr;
		m_pPhysicsManager->Release();
		delete m_pPhysicsManager;
		delete m_pLightManager;
		delete m_pSceneGraph;
	}

	std::map<Physics::Transform*, Renderer::Mesh*> Scene::GetObjects()
	{
		return m_pSceneGraph->GetObjects();
	}

	std::map<Physics::Transform*, Renderer::Mesh*> Scene::GetCullObjects()
	{
		return m_pSceneGraph->Cull();
	}

	void Scene::Load()
	{
		bool resourcesLoaded = false;
		bool newGameObjectLoaded = false;
		GameObject* newGameObject = nullptr;

		std::string content;
		if (Utils::ReadFile(m_path.string(), content))
		{
			std::istringstream is(content);
			std::string line;


			while (std::getline(is, line))
			{
				std::stringstream sistream;
				sistream.str(line);

				if (!resourcesLoaded)
				{
					if (line == "***")
					{
						resourcesLoaded = true;
						continue;
					}

					std::string loadedContent[3];

					for (int i = 0; i < 3; i++)
						std::getline(sistream, loadedContent[i], ',');

					Resources::ResourceManager::CreateSpecificResource((Resources::ResourceType)std::stoi(loadedContent[0]), loadedContent[1].c_str(), loadedContent[2].c_str());
					continue;
				}

				if (!newGameObjectLoaded)
				{
					newGameObject = m_pSceneGraph->AddGameObject(line.c_str());
					newGameObjectLoaded = true;
					std::getline(is, line);
					if (std::stoi(line) == 0)
					{
						(*newGameObject->GetState()) = false;
						newGameObject->UpdateState();
					}
					continue;
				}

				if (line == "***")
				{
					newGameObjectLoaded = false;
					continue;
				}

				newGameObject->Load(line);
			}
		}
	}

	void Scene::SetCamera()
	{
		if (m_pMainCam) m_pMainCam->isMainCamera = false;
		SceneManagement::GameObject* cam = m_pSceneGraph->GetFirstCameraOfScene();
		m_pCamTranform = cam->GetTransform();
		m_pMainCam = cam->GetComponent<CameraRender::Camera>();
		m_pMainCam->isMainCamera = true;
		SetSkybox(m_pMainCam->GetSkyBox());
	}

	CameraRender::SkyBox* Scene::GetSkybox()
	{
		return m_pCurrentSkybox;
	}

	void Scene::SetSkybox(CameraRender::SkyBox* sky)
	{
		m_pCurrentSkybox = sky;

	}

	void  Scene::SetCamera(CameraRender::Camera* cam)
	{
		m_pMainCam->isMainCamera = false;
		m_pCamTranform = cam->GetGameObject()->GetTransform();
		m_pMainCam = cam;
	}

	void Scene::RemoveCamera()
	{
		m_pCamTranform = nullptr;
		m_pMainCam = nullptr;
	}

	void Scene::SetName(const char* name)
	{
		m_name = name;
		std::string newPath = m_path.parent_path().string();
		newPath += "/";
		newPath += name;
		newPath += ".lscene";
		m_path = newPath;
	}

	std::string Scene::WriteChildGameObjectData(GameObject* go)
	{
		std::string savedContent;
		Physics::Transform* transform = go->GetTransform();
		std::vector<Physics::Transform*> arrayTransform = transform->GetChildrenArray();
		for (std::vector<Physics::Transform*> ::iterator index = arrayTransform.begin(); index != arrayTransform.end(); index++)
		{
			savedContent += (*index)->GetGameObject()->Save();
			savedContent += "\n***\n";
			savedContent += WriteChildGameObjectData((*index)->GetGameObject());
		}

		return savedContent;
	}

	void Scene::SaveResource(std::string& savedContent, std::unordered_map<std::string, Resources::Resource*>& resourceUsed, Resources::Resource* resource)
	{
		if (resource == nullptr)
			return;

		if (resourceUsed.count(resource->name))
			return;

		resourceUsed.emplace(resource->name, resource);
		savedContent += resource->Save() + "\n";
	}

	void Scene::Save()
	{
		std::map<std::string, GameObject*> node = m_pSceneGraph->GetSceneNodes();
		std::unordered_map<std::string, Resources::Resource*> resourcesUsed;
		std::string savedContent;

		for (std::map<std::string, GameObject*>::iterator index = node.begin(); index != node.end(); index++)
		{
			Renderer::Mesh* mesh = nullptr;
			if (index->second->HasComponent<Renderer::Mesh>())
				mesh = index->second->GetComponent<Renderer::Mesh>();

			if (index->second->HasComponent<CameraRender::Camera>() && index->second->GetComponent<CameraRender::Camera>()->GetSkyBox() != nullptr)
			{
				CameraRender::SkyBox* skybox = index->second->GetComponent<CameraRender::Camera>()->GetSkyBox();
				std::vector<Resources::Resource*> faces = skybox->GetTexturesResources();

				for (std::vector<Resources::Resource*>::iterator it = faces.begin(); it != faces.end(); it++)
					SaveResource(savedContent, resourcesUsed, (*it));

				SaveResource(savedContent, resourcesUsed, skybox->GetModel());
				SaveResource(savedContent, resourcesUsed, skybox);
			}

			if (index->second->HasComponent<UI::Text>() && index->second->GetComponent<UI::Text>()->GetFont() != nullptr)
			{
				UI::Font* font = index->second->GetComponent<UI::Text>()->GetFont();
				SaveResource(savedContent, resourcesUsed, font);
			}

			if (index->second->HasComponent<UI::Image>() && index->second->GetComponent<UI::Image>()->GetTexture() != nullptr)
			{
				Resources::Texture* texture = index->second->GetComponent<UI::Image>()->GetTexture();
				SaveResource(savedContent, resourcesUsed, texture);
			}

			if (index->second->HasComponent<SoundSystem::SoundEvent>())
			{
				SoundSystem::SoundEvent* soundComponent =  index->second->GetComponent<SoundSystem::SoundEvent>();
				for (size_t i = 0; i < soundComponent->GetSoundCount(); i++)
				{
					if (soundComponent->GetResource(i) != nullptr)
					{
						Resources::SoundInstance* sound = index->second->GetComponent<SoundSystem::SoundEvent>()->GetResource(i);
						SaveResource(savedContent, resourcesUsed, sound);
					}
				}
				
			}


			if (mesh == nullptr) continue;

			Resources::Model* model = mesh->GetModel();
			std::vector<Resources::Material*> materials = mesh->GetMaterials();

			for (std::vector<Resources::Material*>::iterator indexMat = materials.begin(); indexMat != materials.end(); indexMat++)
			{
				if ((*indexMat) == nullptr) continue;

				std::vector<Resources::Texture*> materialsText = (*indexMat)->GetTextures();
				for (std::vector<Resources::Texture*>::iterator indexText = materialsText.begin(); indexText != materialsText.end(); indexText++)
					SaveResource(savedContent, resourcesUsed, (*indexText));

				SaveResource(savedContent, resourcesUsed, (*indexMat));
			}

			if (model != nullptr)
				SaveResource(savedContent, resourcesUsed, model);

			if (index->second->HasComponent<AnimationSystem::Animator>())
			{
				std::vector<Resources::Resource*> animatorResources = index->second->GetComponent<AnimationSystem::Animator>()->GetResourcesUsed();

				for (std::vector<Resources::Resource*>::iterator indexAnim = animatorResources.begin(); indexAnim != animatorResources.end(); indexAnim++)
					SaveResource(savedContent, resourcesUsed, (*indexAnim));
			}
		}

		savedContent += "***\n";

		for (std::map<std::string, GameObject*>::iterator index = node.begin(); index != node.end(); index++)
		{
			if (index->second->GetTransform()->HasParent()) continue;
			savedContent += index->second->Save();
			savedContent += "\n***\n";
			savedContent += WriteChildGameObjectData(index->second);
		}

		Utils::WriteFile(m_path.string(), savedContent);
		LDEBUG("Scene has been save succesfully");
	}

	void Scene::Unload()
	{
		m_pSceneGraph->CleanUp();
		delete m_pSceneGraph;
		m_pSceneGraph = nullptr;
		m_pCamTranform = nullptr;
		m_pMainCam = nullptr;
	}

	void Scene::Update()
	{
		if (m_pMainCam != nullptr)
		{
			m_pMainCam->CalculateMatrix();
			m_pSceneGraph->Update(m_pMainCam, m_isPlaying);
			m_pPhysicsManager->RunSimulation();
		}
	}

	GameObject* Scene::GetGameObject(std::string name)
	{
		return m_pSceneGraph->GetGameObject(name.c_str());
	}
}