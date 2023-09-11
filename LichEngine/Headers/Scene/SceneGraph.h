#pragma once
#include <Renderer/Mesh.h>
#include <Animation/Animator.h>
#include <Core/InputManager.h>

namespace CameraRender { class Camera; }
namespace Physics { class Transform; class PhysicsManager; }
namespace LightSystem { class Light; }

namespace SceneManagement
{

	struct ObjInfo
	{
		Renderer::Mesh * mesh;
		Matrix4D model;
		Matrix4D mvp;
		AnimationSystem::Animator* animator;
		bool isDraw;
		Vector3D globalPosition;
	};

	class GameObject;
	class Scene;

	class LICH_ENGINE SceneGraph
	{
	public:
		SceneGraph(Physics::PhysicsManager* physicsManager, Scene* pScene);
		~SceneGraph();

		void Update(CameraRender::Camera* cam, bool isPlaying);
		GameObject* AddGameObject(const char* name);
		void CreateGameObject(const char* name, GameObject* obj);
		void RemoveGameObject(GameObject* objPtr);
		void DuplicateGameObject(GameObject* objPtr);
		const char* GetSceneName();
		GameObject* GetGameObject(const char* name);
		Core::InputManager* GetInputManager();
		void CalculateSceneIndex();
		void SetSceneChildIndex(std::vector<Physics::Transform*> children, int& index);;
		std::map<Physics::Transform*, Renderer::Mesh*> Cull();
		void CleanUp();
		std::map<std::string, GameObject*> GetSceneNodes() { return m_sceneNode; }
		std::map<Physics::Transform*, Renderer::Mesh*> GetObjects() { return m_objects; }
		std::vector<Physics::Transform*>* GetTransformArray() { return &m_nodesArray; }
		std::string& GetName(int index);
		Scene* GetScene() { return m_pScene; }
		Physics::PhysicsManager* GetPhysicsManager() { return m_pPhysicsManager; }

		bool isNameIsUse(const char* name);

		void AddLight(LightSystem::Light* light);
		void RemoveLight(LightSystem::Light* light);
		void AddMeshToDraw(Physics::Transform* transform, Renderer::Mesh* mesh) { m_objects[transform] = mesh; }
		void RemoveMesh(Physics::Transform* transform) { m_objects.erase(transform); }
		void RemoveCamera(CameraRender::Camera* camPtr);

		GameObject* GetFirstCameraOfScene();
		std::vector<ObjInfo*> GetObjInfo() { return m_pObjInfo; }
	private:
		void UpdateChild(std::vector<Physics::Transform*> children, CameraRender::Camera* cam, bool isPlaying);
		GameObject* AddGameObjectProcess(const char* name);
		Vector4D GiveGameObjectId();
		Scene* m_pScene;
		std::map<Physics::Transform*, Renderer::Mesh*> m_objects;
		std::map<std::string, GameObject*> m_sceneNode;
		std::vector<Physics::Transform*>  m_nodesArray;
		Physics::PhysicsManager* m_pPhysicsManager;
		std::vector<ObjInfo*> m_pObjInfo;
		float cullDistance = 20.0f;

		std::string m_nameScene;
		Vector4D m_lastGameObjectID = Vector4D(0, 0, 0, 0);
	};
};
