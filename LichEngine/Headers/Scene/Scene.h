#pragma once
#include <Scene/GameObject.h>
#include <Scene/Gizmos.h>
#include <Core/LightManager.h>
#include <Core/InputManager.h>


namespace Physics
{
	class PhysicsManager;
}

namespace SceneManagement
{
	class SceneManager;

	class  LICH_ENGINE Scene
	{
	public:
		Scene();
		Scene(std::string name, std::string path, bool isNew, Core::InputManager* inputManager, SceneManager* sceneManager);
		~Scene();
		void Load();
		void SetCamera();
		CameraRender::SkyBox* GetSkybox();
		void SetSkybox(CameraRender::SkyBox* sky);
		void SetCamera(CameraRender::Camera* cam);
		void RemoveCamera();
		void Save();
		void Unload();

		GameObject* CreateMesh(std::string name, Vector3D position, Vector3D scale);
		void CreateMesh(std::string name, Vector3D position, Vector3D rotation, Vector3D scale);

		void Update();
		void SetName(const char* name);
		void ChangePlayingState()								{ m_isPlaying = !m_isPlaying; }
		void ChangePlayingState(bool state)								{ m_isPlaying = state; }
		void SetInputManager(Core::InputManager* inputManager)	{ m_pInputManager = inputManager; }

		GameObject* GetGameObject(std::string name);

		std::string WriteChildGameObjectData(GameObject* go);
		std::map<Physics::Transform*, Renderer::Mesh*> GetObjects();
		std::map<Physics::Transform*, Renderer::Mesh*> GetCullObjects();
		std::vector<ObjInfo*>  GetObjInfo() {
			m_pSceneGraph->Cull();
			return m_pSceneGraph->GetObjInfo();
		}

		void ChangeDrawCollidersState() { m_drawAllCollider = !m_drawAllCollider; }

		// Getter -----------------------------------
		char* GetNameData() { return m_name.data(); }
		bool IsPlaying() { return m_isPlaying; }
		std::string GetName() { return m_name; }
		std::filesystem::path GetPath() { return m_path; }
		std::map<std::string, GameObject*> GetGameObjects() { return m_pSceneGraph->GetSceneNodes(); }
		Gizmos* GetGizmos() { return m_pGizmos; }
		SceneGraph* GetSceneGraph() { return m_pSceneGraph; }
		CameraRender::Camera* GetCamera() { return m_pMainCam; }
		Core::InputManager* GetInputManager() { return m_pInputManager; }
		Physics::Transform* GetCameraTransform() { return m_pCamTranform; }
		LightSystem::LightManager* GetLightManager() { return m_pLightManager; }
		Physics::PhysicsManager* GetPhysicManager() { return m_pPhysicsManager; }
		bool GetDrawCollidersState() { return m_drawAllCollider; }
		SceneManager* GetSceneManager() { return m_pSceneManager; }

	private:
		LightSystem::LightManager* m_pLightManager;
		Core::InputManager* m_pInputManager;
		Gizmos* m_pGizmos;
		SceneGraph* m_pSceneGraph;
		Physics::PhysicsManager* m_pPhysicsManager;
		CameraRender::Camera* m_pMainCam;
		Physics::Transform* m_pCamTranform;
		SceneManager* m_pSceneManager;

		std::string	 m_name;
		std::filesystem::path m_path;
		CameraRender::SkyBox* m_pCurrentSkybox;

		void SaveResource(std::string& savedContent, std::unordered_map<std::string, Resources::Resource*>& resourceUsed, Resources::Resource* resource);
		bool m_drawAllCollider = false;
		bool m_isPlaying = true;
	};
};