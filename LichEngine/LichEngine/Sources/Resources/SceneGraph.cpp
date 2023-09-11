#include <pch.h>
#include <Core/Light.h>
#include <Scene/Scene.h>
#include <Scene/SceneGraph.h>
#include <Physics/RigidBody.h>
#include <Resources/Scripting.h>
#include <Physics/PhysicsManager.h>


namespace SceneManagement
{
	SceneGraph::SceneGraph(Physics::PhysicsManager* physicsManager, Scene* pScene)
	{
		m_pPhysicsManager = physicsManager;
		m_pScene = pScene;
		m_nameScene = m_pScene->GetName();
	}

	SceneGraph::~SceneGraph()
	{
		m_pScene = nullptr;
		m_pPhysicsManager = nullptr;

		m_nodesArray.clear();
		m_objects.clear();

		for (std::map<std::string, GameObject*>::iterator index = m_sceneNode.begin(); index != m_sceneNode.end(); index++)
			delete(index->second);

		m_sceneNode.clear();
	}

	void SceneGraph::Update(CameraRender::Camera* cam, bool isPlaying)
	{
		if (isPlaying)
		{
			for (auto iter = m_nodesArray.begin(); iter != m_nodesArray.end(); iter++)
			{
				if ((*iter)->GetGameObject()->HasComponent<Physics::RigidBody>())
					(*iter)->PostPhysicUpdate();
			}

			for (auto iter = m_nodesArray.begin(); iter != m_nodesArray.end(); iter++)
			{
				if ((*iter)->GetGameObject()->HasComponent<Resources::Scripting>())
				{
					//Called FixedUpdate
					Resources::Scripting* script = (*iter)->GetGameObject()->GetComponent<Resources::Scripting>();
					
					if (script != nullptr)
						script->FixedUpdateScript();
				}
			}
		}

		for (auto iter = m_nodesArray.begin(); iter != m_nodesArray.end(); iter++)
		{
			GameObject* currentGo = (*iter)->GetGameObject();
			if (currentGo->IsUpdate() || (*iter)->HasParent())
				continue;

			(*iter)->Update(cam, false, isPlaying);
		}

		for (auto iter = m_nodesArray.begin(); iter != m_nodesArray.end(); iter++)
			(*iter)->ResetUpdateState();
	}

	std::string& SceneGraph::GetName(int index)
	{
		return (m_nodesArray[index]->GetGameObject()->GetName());
	}

	bool SceneGraph::isNameIsUse(const char* name)
	{
		if (m_sceneNode.find(name) == m_sceneNode.end()) return false;
		else return true;

	}

	void SceneGraph::UpdateChild(std::vector<Physics::Transform*> children, CameraRender::Camera* cam, bool isPlaying)
	{

		for (std::vector<Physics::Transform*>::iterator index = children.begin(); index != children.end(); index++)
		{
			(*index)->Update(cam, false, isPlaying);
			std::vector<Physics::Transform*> children = (*index)->GetChildrenArray();

			if (children.size() != 0)
				UpdateChild(children, cam, isPlaying);
		}
	}

	GameObject* SceneGraph::AddGameObjectProcess(const char* name)
	{
		GameObject* obj = new GameObject(name, this, m_pPhysicsManager);
		obj->SetId(GiveGameObjectId());
		obj->GetTransform()->SetSceneIndex((int)m_nodesArray.size());
		m_nodesArray.push_back(obj->GetTransform());
		m_sceneNode[(obj->GetName())] = obj;
		return obj;
	}



	GameObject* SceneGraph::AddGameObject(const char* name = "GameObject")
	{
		std::string nameStr = name;
		if (nameStr == "GameObject")
			nameStr = nameStr + std::to_string(m_nodesArray.size());
		return AddGameObjectProcess(nameStr.c_str());

	}

	void SceneGraph::CreateGameObject(const char* name, GameObject* obj)
	{
		(*obj) = SceneManagement::GameObject(name, this, m_pPhysicsManager);
		obj->SetId(GiveGameObjectId());
		obj->GetTransform()->SetSceneIndex((int)m_nodesArray.size());
		m_nodesArray.push_back(obj->GetTransform());
		m_sceneNode[(obj->GetName())] = obj;
	}

	void SceneGraph::RemoveGameObject(GameObject* objPtr)
	{
		Physics::Transform* transformPtr = objPtr->GetTransform();
		objPtr->RemoveParent();
		transformPtr->RemoveChilds();
		if (Utils::EraseVectorElement<Physics::Transform*>(m_nodesArray, transformPtr))
		{
			auto iter = m_sceneNode.find((objPtr->GetName()));
			if (iter != m_sceneNode.end())	m_sceneNode.erase(iter);
			delete objPtr;
		}
	}
	void SceneGraph::DuplicateGameObject(GameObject* objPtr)
	{
		std::string name = objPtr->GetName() + "1";
		int indexObj = 1;
		while (isNameIsUse(name.c_str()))
		{
			indexObj++;
			name = objPtr->GetName() + std::to_string(indexObj);
		}

		GameObject* obj = new SceneManagement::GameObject(name.c_str(), this, m_pPhysicsManager);
		obj->SetId(GiveGameObjectId());
		obj->GetTransform()->SetSceneIndex((int)m_nodesArray.size());
		m_nodesArray.push_back(obj->GetTransform());
		m_sceneNode[(obj->GetName())] = obj;

		(*obj->GetTransform()).SetLocalPosition((*objPtr->GetTransform()).GetLocalPosition());
		(*obj->GetTransform()).SetLocalRotation((*objPtr->GetTransform()).GetLocalRotation());
		(*obj->GetTransform()).SetLocalScale((*objPtr->GetTransform()).GetLocalScale());

		std::vector<Component*> comps = (*objPtr->GetComponents());
		std::vector<Component*>* compsNew = obj->GetComponents();
		for (int i = 0; i < comps.size(); i++)
		{
			obj->AddSpecificComponent(comps[i]->componentType, true);
			objPtr->CopyComponent(i, (*compsNew)[i]);
		}

	}
	const char* SceneGraph::GetSceneName() { return (m_nameScene.c_str()); }

	GameObject* SceneGraph::GetGameObject(const char* name)
	{
		return m_sceneNode.find(name)->second;
	}

	Core::InputManager* SceneGraph::GetInputManager()
	{
		return m_pScene->GetInputManager();
	}

	void SceneGraph::CalculateSceneIndex()
	{
		int index = 0;
		for (size_t i = 0; i < m_nodesArray.size(); i++)
		{
			if (m_nodesArray[i]->HasParent()) continue;
			m_nodesArray[i]->SetSceneIndex(index);
			index++;
			SetSceneChildIndex(m_nodesArray[i]->GetChildrenArray(),index);
		}
	}

	void SceneGraph::SetSceneChildIndex(std::vector<Physics::Transform*> children,int& index)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->SetSceneIndex(index);
			index++;
			SetSceneChildIndex(children[i]->GetChildrenArray(), index);
		}
	}

	std::map<Physics::Transform*, Renderer::Mesh*> SceneGraph::Cull()
	{
		m_pObjInfo.clear();
		std::map<Physics::Transform*, Renderer::Mesh*> cullMeshes;
		for (std::map<Physics::Transform*, Renderer::Mesh*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if ((m_pScene->GetCameraTransform()->GetGlobalPosition() - it->first->GetGlobalPosition()).Magnitude() >= cullDistance)
				continue;

			if (it->second->GetModel() != nullptr && it->second->GetModel()->IsCreated())
			{
				ObjInfo* info = new ObjInfo();
				info->mesh = it->second;
				info->mvp = it->first->GetMVP();
				info->model = it->first->GetModelMatrice();
				info->animator = it->first->GetGameObject()->GetComponent<AnimationSystem::Animator>();
				info->isDraw = it->first->GetLocalScale().x == 0;
				info->globalPosition = it->first->GetGlobalPosition();
				m_pObjInfo.push_back(info);
				cullMeshes.emplace((*it));
			}
		}

		

		return cullMeshes;
	}

	void SceneGraph::CleanUp()
	{
		m_objects.clear();
		m_nodesArray.clear();

		for (std::map<std::string, GameObject*>::iterator index = m_sceneNode.begin(); index != m_sceneNode.end(); index++)
		{
			GameObject* currentGo = index->second;
			delete currentGo;
		}

		m_sceneNode.clear();
	}

	void SceneGraph::AddLight(LightSystem::Light* light)
	{
		m_pScene->GetLightManager()->AddLight(light);
	}

	void SceneGraph::RemoveLight(LightSystem::Light* light)
	{
		m_pScene->GetLightManager()->RemoveLight(light);
	}

	void SceneGraph::RemoveCamera(CameraRender::Camera* cam)
	{
		if (cam == m_pScene->GetCamera())
		{
			m_pScene->RemoveCamera();
		}
	}
	GameObject* SceneGraph::GetFirstCameraOfScene()
	{
		for (std::map<std::string, GameObject*>::iterator index = m_sceneNode.begin(); index != m_sceneNode.end(); index++)
		{
			if ((*index).second->HasComponent<CameraRender::Camera>())
			{
				return (*index).second;
			}
		}

	}

	Vector4D SceneGraph::GiveGameObjectId()
	{
		m_lastGameObjectID.x += 10;
		if (m_lastGameObjectID.x > 255)
		{
			m_lastGameObjectID.x = 0;
			m_lastGameObjectID.y += 10;
		}
		if (m_lastGameObjectID.y > 255)
		{
			m_lastGameObjectID.y = 0;
			m_lastGameObjectID.z += 10;
		}
		if (m_lastGameObjectID.z > 255)
		{
			m_lastGameObjectID.z = 0;
			m_lastGameObjectID.w += 10;
		}
		return m_lastGameObjectID;
	}
}