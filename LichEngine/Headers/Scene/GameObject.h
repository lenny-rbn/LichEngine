#pragma once
#include <Debugs/Log.h>
#include <Camera/Camera.h>
#include <Scene/SceneGraph.h>
#include <Physics/Transform.h>
#include <Core/InputManager.h>
#include <Generated/GameObject.rfkh.h>

namespace LightSystem { class Light; }
namespace Resources { class Scripting; }
namespace Physics { class PhysicsManager; }
namespace UI { class Text; }


namespace SceneManagement NAMESPACE()
{
	class CLASS() LICH_ENGINE GameObject
	{
	private:
		std::vector<Component*> m_components;
		std::vector<Component*> m_componentsToDestroy;
		std::vector<Component*> m_componentsToAdd;
		Physics::Transform* m_pTransform;
		Renderer::Mesh* m_pMesh;
		std::string m_name;
		
		SceneManagement::SceneGraph* m_pSceneGraph;
		Physics::PhysicsManager* m_pPhysicsManager;
		Vector4D m_id;
		bool m_hasBeenUpdate = false;
		bool m_isActive = true;
		bool m_isSelected = false;

		bool* m_componentState = new bool[100];
		unsigned int  m_componentStateSize = 0;

		void DestroyComponents();
		void AddComponents();

	
		void SetScripting(Component* comp);
		void AddPhysicalComponent(Component* comp);
		void RemovePhysicalComponent(Component* comp);

	public:
		GameObject(const char* name = "GameObject_1", SceneGraph* graph = nullptr, Physics::PhysicsManager* physicsManager = nullptr);
		~GameObject();

		void Update(CameraRender::Camera* cam, bool isPlaying);
		std::string Save();
		void Load(std::string content);

		void UpdateState();
		bool* GetState()								{ return &m_isActive; }
		void SetActive()								{ m_isActive = !m_isActive; }

		Physics::Transform* GetTransform()				{ return m_pTransform; };
		std::string& GetName()							{ return m_name; }
		std::string* GetNamePtr()						{ return &m_name; }
		void RestoreName()								{ m_name = prevName; }
		SceneManagement::SceneGraph* GetSceneGraph()	{ return m_pSceneGraph; }
		Physics::PhysicsManager* GetPhysicsManager()	{ return m_pPhysicsManager; }
		bool IsUpdate()									{ return m_hasBeenUpdate; }
		std::vector<Component*>* GetComponents()		{ return &m_components; }

		inline Core::InputManager* GetInputManager()    { return m_pSceneGraph->GetInputManager(); }

		Vector4D GetId()								{ return m_id; };
		void SetId(Vector4D newId)						{ m_id = newId; };
		bool IsSelected()								{ return m_isSelected; }
		void SetSelectedState(bool state)				{ m_isSelected = state; }

		std::string prevName;

		bool m_isPlaying = false;

		void SetMesh(Resources::Model* model);

		void SetParent(GameObject* obj);;
		void SetChildren(GameObject* obj);

		int GetComponentIndex(SceneManagement::ComponentType type);

		void RemoveAllComponent();
		void RemoveParent();
		void RemoveChild(GameObject* obj);

		template <typename T>
		T* AddComponent(bool startGame = false)
		{
			if (std::is_base_of<Component, T>::value)
			{
				//Check is the component already exists
				if (HasComponent<T>())
				{
					LWARNING("Component already exists");
					return GetComponent<T>();
				}

				T* comp = new T(this);
				if (!startGame)
					m_componentsToAdd.push_back(comp);
				else
					m_components.push_back(comp);

				if (dynamic_cast<Renderer::Mesh*>(comp) != nullptr)
				{
					m_pMesh = (Renderer::Mesh*)comp;
					m_pSceneGraph->AddMeshToDraw(m_pTransform, m_pMesh);
				}
				if (dynamic_cast<LightSystem::Light*>(comp) != nullptr)
				{
					LightSystem::Light* light = (LightSystem::Light*)comp;
					light->SetPosition(m_pTransform->GetGlobalPosition());
					m_pSceneGraph->AddLight(light);
				}

				SetScripting(comp);
				AddPhysicalComponent(comp);

				return comp;
			}
		}

		template <typename T>
		void RemoveComponent(ComponentType type)
		{
			for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
			{
				if (dynamic_cast<T*>(*iter) != nullptr)
				{
					T* comp = (T*)(*iter);
					m_componentsToDestroy.push_back(comp);
					if (dynamic_cast<Renderer::Mesh*>(comp) != nullptr)
					{
						m_pSceneGraph->RemoveMesh(m_pTransform);
						m_pMesh = nullptr;
					}
					if (dynamic_cast<LightSystem::Light*>(comp) != nullptr)
					{
						LightSystem::Light* light = (LightSystem::Light*)(comp);
						m_pSceneGraph->RemoveLight(light);
					}
					if (dynamic_cast<CameraRender::Camera*>(comp) != nullptr)
					{
						CameraRender::Camera* cam = (CameraRender::Camera*)(comp);
						m_pSceneGraph->RemoveCamera(cam);
					}
					RemovePhysicalComponent((*iter));


					return;
				}
			}

			LWARNING("Component doesn't exists");
		}

		template <typename T>
		T* GetComponent()
		{
			for (int i = 0; i < m_components.size(); i++)
			{
				T* component = dynamic_cast<T*>(m_components[i]);
				if (component != nullptr)
					return component;
			}
			for (int i = 0; i < m_componentsToAdd.size(); i++)
			{
				T* component = dynamic_cast<T*>(m_componentsToAdd[i]);
				if (component != nullptr)
					return component;
			}

		
			return nullptr;
		}

		template <typename T>
		bool HasComponent()
		{
			for (int i = 0; i < m_components.size(); i++)
			{
				T* component = dynamic_cast<T*>(m_components[i]);
				if (component != nullptr)
					return true;
			}
			for (int i = 0; i < m_componentsToAdd.size(); i++)
			{
				T* component = dynamic_cast<T*>(m_componentsToAdd[i]);
				if (component != nullptr)
					return true;
			}

			return false;
		}

		void AddSpecificComponent(int componentType, std::string content, bool startGame = false);
		void AddSpecificComponent(int componentType, bool start);
		void CopyComponent(int indexComponent, Component* comp);

		void OnCollisionEnter(GameObject* go);
		void OnCollisionStay(GameObject* go);
		void OnCollisionExit(GameObject* go);
		void OnTriggerEnter(GameObject* go);
		void OnTriggerExit(GameObject* go);
		void RemoveSpecificComponent(SceneManagement::ComponentType type);

		bool HasSpecificComponent(int componentType);

		SceneManagement_GameObject_GENERATED
	};
}
File_GameObject_GENERATED