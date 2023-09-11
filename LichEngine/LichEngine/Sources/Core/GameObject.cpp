#include <pch.h>
#include <sstream>

#include <UI/Text.h>
#include <UI/Image.h>
#include <Renderer/Mesh.h>
#include <Camera/Camera.h>
#include <Core/DirLight.h>
#include <Core/SpotLight.h>
#include <Core/PointLight.h>
#include <Physics/Collider.h>
#include <Physics/RigidBody.h>
#include <Core/LightManager.h>
#include <Resources/Scripting.h>
#include <Animation/Animator.h>
#include <Physics/PhysicsManager.h>
#include <Sound/SoundListner.h>
#include <Sound/SoundEvent.h>

#include <Generated/GameObject.rfks.h>

namespace SceneManagement
{
	void GameObject::DestroyComponents()
	{
		if (m_componentsToDestroy.size() == 0) return;

		for (size_t j = 0; j < m_componentsToDestroy.size(); j++)
		{
			for (size_t i = 0; i < m_components.size(); i++)
			{
				if (m_componentsToDestroy[j] == m_components[i])
					m_components.erase(m_components.begin() + i);
			}
		}

		for (size_t i = 0; i < m_componentsToDestroy.size(); i++)
		{
			Component* comp = m_componentsToDestroy[i];
			delete comp;
		}

		m_componentsToDestroy.clear();
	}

	void GameObject::AddComponents()
	{
		if (m_componentsToAdd.size() == 0) return;

		for (size_t i = 0; i < m_componentsToAdd.size(); i++)
			m_components.push_back(m_componentsToAdd[i]);

		m_componentsToAdd.clear();
	}

	void GameObject::SetScripting(Component* comp)
	{
		if (dynamic_cast<Resources::Scripting*>(comp) != nullptr)
		{
			Resources::Scripting* script = dynamic_cast<Resources::Scripting*>(comp);
			script->input = m_pSceneGraph->GetInputManager();
			script->SetGameObject(this);
		}
	}

	void GameObject::AddPhysicalComponent(Component* comp)
	{
		if (dynamic_cast<Physics::Collider*>(comp) != nullptr)
			m_pPhysicsManager->AddPhysicsActor(this);

		if (dynamic_cast<Physics::RigidBody*>(comp) != nullptr)
			if (!HasComponent<Physics::Collider>())
				AddComponent<Physics::Collider>(true);
	}

	void GameObject::RemovePhysicalComponent(Component* comp)
	{
		if (dynamic_cast<Physics::Collider*>(comp) != nullptr)
			m_pPhysicsManager->DeletePhysicsActor(this);

		if (dynamic_cast<Physics::RigidBody*>(comp) != nullptr)
		{

		}
	}

	GameObject::GameObject(const char* name, SceneGraph* graph, Physics::PhysicsManager* physicsManager) : m_name(name)
	{
		prevName = name;
		m_pTransform = new Physics::Transform(this);
		m_pMesh = nullptr;
		m_pSceneGraph = graph;
		m_pPhysicsManager = physicsManager;
	}

	GameObject::~GameObject()
	{
		delete m_pTransform;

		m_pMesh = nullptr;

		RemoveAllComponent();

		for (int i = 0; i < m_componentsToAdd.size(); i++)
			delete m_componentsToAdd[i];

		m_componentsToAdd.clear();
		m_pSceneGraph = nullptr;
		m_pPhysicsManager = nullptr;
	}

	void GameObject::Update(CameraRender::Camera* cam, bool isPlaying)
	{
		DestroyComponents();
		AddComponents();
		m_isPlaying = isPlaying;

		for (std::vector<Component*>::iterator iter = m_components.begin(); iter != m_components.end(); iter++)
			(*iter)->Update((*m_pTransform));
	}

	std::string GameObject::Save()
	{
		std::string content = m_name + "\n";
		content += std::to_string(m_isActive) + "\n";
		content += m_pTransform->SaveData();

		for (int i = 0; i < m_components.size(); i++)
			content += m_components[i]->SaveData();

		return content;
	}

	void GameObject::Load(std::string content)
	{
		std::stringstream sistream;
		sistream.str(content);
		std::string loadedContent;

		bool transformLoaded = false;
		while (std::getline(sistream, loadedContent, '~'))
		{
			if (loadedContent == "")
				continue;

			size_t id = loadedContent.find_first_of(',');
			std::string componentId = loadedContent.substr(0, id);
			std::string componentContent = loadedContent.substr(id + 1);

			if (!transformLoaded)
			{
				m_pTransform->LoadData(componentContent);
				transformLoaded = true;
				continue;
			}

			AddSpecificComponent((ComponentType)std::stoi(componentId), componentContent, true);
			m_components[m_components.size() - 1]->isEnable = m_isActive;
		}
	}

	void GameObject::UpdateState()
	{
		int currentComponent = 0;
		if (m_isActive)
		{
			for (auto component : m_components)
			{
				component->isEnable = m_componentState[currentComponent];
				component->ChangeEnableState();
				currentComponent++;
			}
			for (auto child : m_pTransform->GetChildrenArray())
			{
				bool* state = child->GetGameObject()->GetState();
				(*state) = m_isActive;
				child->GetGameObject()->UpdateState();
			}
			return;
		}

		m_componentStateSize = (unsigned int)m_components.size();

		for (auto component : m_components)
		{
			m_componentState[currentComponent] = component->isEnable;
			component->isEnable = false;
			component->ChangeEnableState();
			currentComponent++;
		}

		for (auto child : m_pTransform->GetChildrenArray())
		{
			bool* state = child->GetGameObject()->GetState();
			(*state) = m_isActive;
			child->GetGameObject()->UpdateState();
		}
	}

	void GameObject::SetMesh(Resources::Model* model)
	{
		m_pMesh->SetModel(model);
	}

	void GameObject::SetParent(GameObject* obj)
	{
		m_pTransform->AddParent(obj->GetTransform());
	}

	void GameObject::SetChildren(GameObject* obj)
	{
		m_pTransform->AddChild(obj->GetTransform());
	}

	int GameObject::GetComponentIndex(SceneManagement::ComponentType type)
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->componentType == type)
			{
				return i;
			}
		}
		for (int i = 0; i < m_componentsToAdd.size(); i++)
		{
			if (m_componentsToAdd[i]->componentType == type)
			{
				return i;
			}
		}
		return -1;
	}

	void GameObject::RemoveAllComponent()
	{
		for (auto component : m_components)
			RemoveSpecificComponent(component->componentType);

		m_components.clear();

		for (int i = 0; i < m_componentsToDestroy.size(); i++)
			delete m_componentsToDestroy[i];

		m_componentsToDestroy.clear();
	}

	void GameObject::RemoveParent()
	{
		m_pTransform->RemoveParent(true);
	}

	void GameObject::RemoveChild(GameObject* obj)
	{
		m_pTransform->RemoveChild(obj->GetTransform());
	}

	void GameObject::AddSpecificComponent(int componentType, std::string content, bool startGame)
	{
		Component* newComponent = nullptr;

		switch (componentType)
		{
		case ComponentType::CAMERA:
			newComponent = AddComponent<CameraRender::Camera>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::DIR_LIGHT:
			newComponent = AddComponent<LightSystem::DirLight>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::POINT_LIGHT:
			newComponent = AddComponent<LightSystem::PointLight>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::SPOT_LIGHT:
			newComponent = AddComponent<LightSystem::SpotLight>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::MESH:
			newComponent = AddComponent<Renderer::Mesh>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::RIGIDBODY:
			newComponent = AddComponent<Physics::RigidBody>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::COLLIDER:
			newComponent = AddComponent<Physics::Collider>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::ANIMATOR:
			newComponent = AddComponent<AnimationSystem::Animator>(startGame);
			newComponent->LoadData(content);
			break;
		case ComponentType::SCRIPTING:
			newComponent = AddComponent<Resources::Scripting>(startGame);
			newComponent->LoadData(content);
			break;
		case SceneManagement::ComponentType::TEXT:
			newComponent = AddComponent<UI::Text>(startGame);
			newComponent->LoadData(content);
			break;
		case SceneManagement::ComponentType::IMAGE:
			newComponent = AddComponent<UI::Image>(startGame);
			newComponent->LoadData(content);
			break;
		case SceneManagement::ComponentType::SOUNDLISTNER:
			newComponent = AddComponent<SoundSystem::SoundListner>(startGame);
			newComponent->LoadData(content);
			break;
		case SceneManagement::ComponentType::SOUNDEVENT:
			newComponent = AddComponent<SoundSystem::SoundEvent>(startGame);
			newComponent->LoadData(content);
			break;
		default:
			break;
		}
	}

	void GameObject::OnCollisionEnter(GameObject* go)
	{
		if (this->HasComponent<Resources::Scripting>())
			this->GetComponent<Resources::Scripting>()->OnCollisionEnter(go);
	}

	void GameObject::OnCollisionStay(GameObject* go)
	{
		if (this->HasComponent<Resources::Scripting>())
			this->GetComponent<Resources::Scripting>()->OnCollisionStay(go);
	}

	void GameObject::OnCollisionExit(GameObject* go)
	{
		if (this->HasComponent<Resources::Scripting>())
			this->GetComponent<Resources::Scripting>()->OnCollisionExit(go);
	}

	void GameObject::OnTriggerEnter(GameObject* go)
	{
		if (this->HasComponent<Resources::Scripting>())
			this->GetComponent<Resources::Scripting>()->OnTriggerEnter(go);
	}

	void GameObject::OnTriggerExit(GameObject* go)
	{
		if (this->HasComponent<Resources::Scripting>())
			this->GetComponent<Resources::Scripting>()->OnTriggerExit(go);
	}

	void GameObject::AddSpecificComponent(int componentType, bool start = false)
	{
		Component* newComponent = nullptr;
		switch (componentType)
		{
		case ComponentType::CAMERA:
			newComponent = AddComponent<CameraRender::Camera>(start);
			break;
		case ComponentType::DIR_LIGHT:
			newComponent = AddComponent<LightSystem::DirLight>(start);
			break;
		case ComponentType::POINT_LIGHT:
			newComponent = AddComponent<LightSystem::PointLight>(start);
			break;
		case ComponentType::SPOT_LIGHT:
			newComponent = AddComponent<LightSystem::SpotLight>(start);
			break;
		case ComponentType::MESH:
			newComponent = AddComponent<Renderer::Mesh>(start);
			break;
		case ComponentType::RIGIDBODY:
			newComponent = AddComponent<Physics::RigidBody>(start);
			break;
		case ComponentType::COLLIDER:
			newComponent = AddComponent<Physics::Collider>(start);
			break;
		case ComponentType::ANIMATOR:
			newComponent = AddComponent<AnimationSystem::Animator>(start);
			break;
		case ComponentType::SCRIPTING:
			newComponent = AddComponent<Resources::Scripting>(start);
			break;
		case SceneManagement::ComponentType::SOUNDLISTNER:
			newComponent = AddComponent<SoundSystem::SoundListner>(start);
			break;
		case SceneManagement::ComponentType::SOUNDEVENT:
			newComponent = AddComponent<SoundSystem::SoundEvent>(start);
			break;
		case SceneManagement::ComponentType::TEXT:
			newComponent = AddComponent<UI::Text>(start);
			break;
		case SceneManagement::ComponentType::IMAGE:
			newComponent = AddComponent<UI::Image>(start);
			break;
		default:
			break;
		}
	}

	void GameObject::CopyComponent(int indexComponent, Component* comp)
	{

		switch (comp->componentType)
		{
		case ComponentType::CAMERA:
		{
			CameraRender::Camera* cam = (CameraRender::Camera*)comp;
			(*cam) = (*(CameraRender::Camera*)m_components[indexComponent]);
		}
		break;
		case ComponentType::DIR_LIGHT:
		{
			LightSystem::DirLight* cam = (LightSystem::DirLight*)comp;
			(*cam) = (*(LightSystem::DirLight*)m_components[indexComponent]);
		}
		break;
		case ComponentType::POINT_LIGHT:
		{
			LightSystem::PointLight* cam = (LightSystem::PointLight*)comp;
			(*cam) = (*(LightSystem::PointLight*)m_components[indexComponent]);
		}
		break;
		case ComponentType::SPOT_LIGHT:
		{
			LightSystem::SpotLight* cam = (LightSystem::SpotLight*)comp;
			(*cam) = (*(LightSystem::SpotLight*)m_components[indexComponent]);
		}
		break;
		case ComponentType::MESH:
		{
			Renderer::Mesh* cam = (Renderer::Mesh*)comp;
			(*cam) = (*(Renderer::Mesh*)m_components[indexComponent]);
		}
		break;
		case ComponentType::RIGIDBODY:
		{
			Physics::RigidBody* cam = (Physics::RigidBody*)comp;
			(*cam) = (*(Physics::RigidBody*)m_components[indexComponent]);
		}
		break;
		case ComponentType::COLLIDER:
		{
			Physics::Collider* cam = (Physics::Collider*)comp;
			(*cam).CopyCollider( (Physics::Collider*)m_components[indexComponent]);
		}
		break;
		case ComponentType::ANIMATOR:
		{
			AnimationSystem::Animator* cam = (AnimationSystem::Animator*)comp;
			(*cam) = (*(AnimationSystem::Animator*)m_components[indexComponent]);
		}
		break;
		case ComponentType::SCRIPTING:
		{
			Resources::Scripting* cam = (Resources::Scripting*)comp;
			(*cam) = (*(Resources::Scripting*)m_components[indexComponent]);
		}
		break;
		case SceneManagement::ComponentType::SOUNDLISTNER:
		{
			SoundSystem::SoundListner* cam = (SoundSystem::SoundListner*)comp;
			(*cam) = (*(SoundSystem::SoundListner*)m_components[indexComponent]);
		}
		break;
		case SceneManagement::ComponentType::SOUNDEVENT:
		{
			SoundSystem::SoundEvent* cam = (SoundSystem::SoundEvent*)comp;
			(*cam) = (*(SoundSystem::SoundEvent*)m_components[indexComponent]);
		}
		break;
		case SceneManagement::ComponentType::TEXT:
		{
			UI::Text* cam = (UI::Text*)comp;
			(*cam) = (*(UI::Text*)m_components[indexComponent]);
		}
		break;
		case SceneManagement::ComponentType::IMAGE:
		{
			UI::Image* cam = (UI::Image*)comp;
			(*cam) = (*(UI::Image*)m_components[indexComponent]);
		}
		break;
		default:
			break;
		}
	}

	void GameObject::RemoveSpecificComponent(ComponentType type)
	{
		Component* newComponent = nullptr;
		switch (type)
		{
		case ComponentType::CAMERA:
			RemoveComponent<CameraRender::Camera>(type);
			break;
		case ComponentType::DIR_LIGHT:
			RemoveComponent<LightSystem::DirLight>(type);
			break;
		case ComponentType::POINT_LIGHT:
			RemoveComponent<LightSystem::PointLight>(type);
			break;
		case ComponentType::SPOT_LIGHT:
			RemoveComponent<LightSystem::SpotLight>(type);
			break;
		case ComponentType::MESH:
			RemoveComponent<Renderer::Mesh>(type);
			break;
		case ComponentType::RIGIDBODY:
			RemoveComponent<Physics::RigidBody>(type);
			break;
		case ComponentType::COLLIDER:
			RemoveComponent<Physics::Collider>(type);
			break;
		case ComponentType::ANIMATOR:
			RemoveComponent<AnimationSystem::Animator>(type);
			break;
		case ComponentType::SCRIPTING:
			RemoveComponent<Resources::Scripting>(type);
			break;
		case SceneManagement::ComponentType::SOUNDLISTNER:
			RemoveComponent<SoundSystem::SoundListner>(type);
			break;
		case SceneManagement::ComponentType::SOUNDEVENT:
			RemoveComponent<SoundSystem::SoundEvent>(type);
			break;
		case SceneManagement::ComponentType::TEXT:
			RemoveComponent<UI::Text>(type);
			break;
		case SceneManagement::ComponentType::IMAGE:
			RemoveComponent<UI::Image>(type);
			break;
		default:
			break;
		}
	}

	bool GameObject::HasSpecificComponent(int componentType)
	{
		switch (componentType)
		{
		case ComponentType::CAMERA:
			return HasComponent<CameraRender::Camera>();
			break;
		case ComponentType::DIR_LIGHT:
			return HasComponent<LightSystem::DirLight>();
			break;
		case ComponentType::POINT_LIGHT:
			return HasComponent<LightSystem::PointLight>();
			break;
		case ComponentType::SPOT_LIGHT:
			return HasComponent<LightSystem::SpotLight>();
			break;
		case ComponentType::MESH:
			return HasComponent<Renderer::Mesh>();
			break;
		case ComponentType::RIGIDBODY:
			return HasComponent<Physics::RigidBody>();
			break;
		case ComponentType::COLLIDER:
			return HasComponent<Physics::Collider>();
			break;
		case ComponentType::ANIMATOR:
			return HasComponent<AnimationSystem::Animator>();
			break;
		case ComponentType::SCRIPTING:
			return HasComponent<Resources::Scripting>();
			break;
		default:
			break;
		}
	}
}