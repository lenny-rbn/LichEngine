#include <pch.h>
#include <Physics/Collider.h>
#include <Scene/GameObject.h>
#include <Core/TimeManager.h>
#include <Physics/RigidBody.h>
#include <Physics/PhysicsManager.h>

#define ISART_IP_ADRESS "10.2.203.21"


namespace Physics
{
	Geometry* PhysicsManager::CreateActorGeometry(const ColliderShape& m_shape)
	{
		Geometry* geometry = nullptr;

		switch (m_shape.type)
		{
		case ShapeType::Plane:
			geometry = new PlaneGeometry();
			break;
		case ShapeType::Box:
			geometry = new BoxGeometry(m_shape.scale.x,m_shape.scale.y,m_shape.scale.z);
			break;
		case ShapeType::Sphere:
			geometry = new SphereGeometry(m_shape.scale.x);
			break;
		case ShapeType::Capsule:
			geometry = new CapsuleGeometry(m_shape.scale.x,m_shape.scale.y);
			break;
		default:
			break;
		}

		return geometry;
	}

	void PhysicsManager::Init(SceneManagement::Scene* scenePtr)
	{
		m_pGameScene = scenePtr;
		m_pFoundation = CreateFoundation(m_DefaultAllocatorCallback,m_DefaultErrorCallback);
		if (m_pFoundation == NULL) throw("CreateFoundation failed!");

		//Optional : Everything around the "Pvd" is optionnal and must be deleted for the release. It's for an visual debugger called PhysX Visual Debugger that works with the PhysX lib.
		bool recordMemoryAllocations = true;

		m_pPvd = CreatePvd(*m_pFoundation);
		PvdTransport* transport = CreateDefaultPvdSocketTransport(ISART_IP_ADRESS,5425,100);
		if (!m_pPvd->connect(*transport,PvdInstrumentationFlag::eALL)) LWARNING("Pvd connection failed");

		m_pPhysics = CreatePhysicsSimulator(m_pFoundation,TolerancesScale(),recordMemoryAllocations,m_pPvd);
		if (!m_pPhysics) throw("PxCreatePhysics failed!");

		SceneDesc sceneDesk(m_pPhysics->getTolerancesScale());
		sceneDesk.gravity = physx::PxVec3(0.f,-9.81f,0.f);
		m_pDispatcher = CreateDefaultCpuDispatcher(2);
		sceneDesk.cpuDispatcher = m_pDispatcher;
		sceneDesk.filterShader = contactReportFilterShader;
		SimulationEventCallback* eventCallBack = new SimulationEventCallback();
		eventCallBack->pPhysicsManager = this;
		m_pSimulationEventCallback = eventCallBack;
		sceneDesk.simulationEventCallback = m_pSimulationEventCallback;
		m_pScene = m_pPhysics->createScene(sceneDesk);

		//Again optionnal, for the visual debug
		PvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PvdSceneFlag::eTRANSMIT_CONSTRAINTS,true);
			pvdClient->setScenePvdFlag(PvdSceneFlag::eTRANSMIT_CONTACTS,true);
			pvdClient->setScenePvdFlag(PvdSceneFlag::eTRANSMIT_SCENEQUERIES,true);
		}
	}

	void PhysicsManager::AddPhysicsActor(SceneManagement::GameObject* gO)
	{
		Collider* coll = gO->GetComponent<Collider>();
		Vector3D leScale = gO->GetTransform()->GetGlobalScale();

		if (gO->HasComponent<RigidBody>())
		{
			RigidBody* rb = gO->GetComponent<RigidBody>();
			RigidDynamic* rigidDynamic = m_pPhysics->createRigidDynamic(ToPxTransform(*gO->GetTransform()));
			{
				//Collider setup
				coll->SetPhysicMaterialPointer(m_pPhysics->createMaterial(coll->GetStaticFriction(),coll->GetDynamicFriction(),coll->GetRestitution()));
				ShapeFlags flags = ShapeFlag::eVISUALIZATION | ShapeFlag::eSCENE_QUERY_SHAPE;
				if (coll->GetTriggerState())
					flags |= ShapeFlag::eTRIGGER_SHAPE;
				else
					flags |= ShapeFlag::eSIMULATION_SHAPE;

				Shape* shape = m_pPhysics->createShape(*CreateActorGeometry(coll->GetShape()),*coll->GetPhysicMaterialPointer(),true,flags);
				rigidDynamic->attachShape(*shape);
				coll->SetSimulationShape(shape);

				//RigidBody setup
				rb->UpdateRigidActor(rigidDynamic);
				rigidDynamic->setActorFlag(ActorFlag::eSEND_SLEEP_NOTIFIES,true);
			}
			m_pScene->addActor(*rigidDynamic);
			m_actors[gO] = rigidDynamic;
			return;
		}

		RigidStatic* rigidStatic = m_pPhysics->createRigidStatic(ToPxTransform(*gO->GetTransform()));
		{
			//Collider setup
			coll->SetPhysicMaterialPointer(m_pPhysics->createMaterial(coll->GetStaticFriction(),coll->GetDynamicFriction(),coll->GetRestitution()));
			ShapeFlags flags = ShapeFlag::eVISUALIZATION | ShapeFlag::eSCENE_QUERY_SHAPE;
			if (coll->GetTriggerState())
				flags |= ShapeFlag::eTRIGGER_SHAPE;
			else
				flags |= ShapeFlag::eSIMULATION_SHAPE;

			Shape* shape = m_pPhysics->createShape(*CreateActorGeometry(coll->GetShape()),*coll->GetPhysicMaterialPointer(),true,flags);
			rigidStatic->attachShape(*shape);
			coll->SetSimulationShape(shape);
		}
		m_pScene->addActor(*rigidStatic);
		m_actors[gO] = rigidStatic;
	}

	void PhysicsManager::DeletePhysicsActor(SceneManagement::GameObject* gO)
	{
		std::map<SceneManagement::GameObject*,RigidActor*>::iterator index = m_actors.find(gO);
		if (index == m_actors.end()) return;
		m_pScene->removeActor(*index->second);
		index->second->release();
		m_actors.erase(index);
	}

	void PhysicsManager::RunSimulation()
	{
		if (m_pGameScene->IsPlaying())
		{
			for (std::map<SceneManagement::GameObject*,RigidActor*>::iterator index = m_actors.begin(); index != m_actors.end(); index++)
			{
				Collider* coll = index->first->GetComponent<Collider>();
				if (!coll->isEnable)
					continue;

				index->second->setGlobalPose(ToPxTransform(index->first->GetTransform()->UpdatePhysics()));

				coll->UpdateSimulationShape(index->second, m_pPhysics, index->first->GetTransform()->GetGlobalScale());

				if (index->first->HasComponent<RigidBody>())
					index->first->GetComponent<RigidBody>()->UpdateRigidActor((RigidDynamic*)(index->second));
			}

			double deltaTime = Core::TimeManager::GetDeltaTime();
			if (deltaTime > 1.f / 60.f)
				deltaTime = 1.f / 60.f;

			m_pScene->simulate((physx::PxReal)deltaTime);

			m_pScene->fetchResults(true);

			for (std::map<SceneManagement::GameObject*,RigidActor*>::iterator index = m_actors.begin(); index != m_actors.end(); index++)
			{
				Collider* coll = index->first->GetComponent<Collider>();
				if (!coll->isEnable)
					continue;

				PhysicTransform tr = index->second->getGlobalPose();
				tr.p -= ToPxVec3(coll->GetOffset());
				index->first->GetTransform()->UpdateFromPhysics(ToTransform(tr));

				if (index->first->HasComponent<RigidBody>())
					index->first->GetComponent<RigidBody>()->UpdateFromRigidActor((RigidDynamic*)(index->second));
			}
		}
	}

	void PhysicsManager::Release()
	{
		m_pDispatcher->release();

		delete m_pSimulationEventCallback;

		for (std::map <SceneManagement::GameObject*,RigidActor*>::iterator index = m_actors.begin(); index != m_actors.end(); index++)
		{
			m_pScene->removeActor(*index->second);
			index->second->release();
		}
		m_actors.clear();

		m_pPhysics->release();

		m_pPvd->release();

		m_pFoundation->release();
	}

	SceneManagement::GameObject* Raycast(PhysicsManager* manager,const Vector3D& origin,const Vector3D& direction,const float range)
	{
		RaycastBuffer hit;
		physx::PxVec3 dir = ToPxVec3(direction);
		dir.normalize();
		manager->GetSimulationScene()->raycast(ToPxVec3(origin),dir,range,hit);

		if (!hit.hasBlock)
			return nullptr;

		std::map<SceneManagement::GameObject*,RigidActor*> map = manager->GetSimulationActors();

		for (std::map<SceneManagement::GameObject*,RigidActor*>::iterator index = map.begin(); index != map.end(); index++)
		{
			if (index->second == hit.block.actor)
				return index->first;
		}

		return nullptr;
	}

	SceneManagement::GameObject* Boxcast(PhysicsManager* manager,const Vector3D& halfExtent,const Vector3D& origin,const Vector3D& direction,const float range)
	{
		SweepBuffer hit;
		manager->GetSimulationScene()->sweep(BoxGeometry(ToPxVec3(halfExtent)),ToPxTransform(origin),ToPxVec3(direction),range,hit);

		if (!hit.hasBlock)
			return nullptr;

		std::map<SceneManagement::GameObject*,RigidActor*> map = manager->GetSimulationActors();

		for (std::map<SceneManagement::GameObject*,RigidActor*>::iterator index = map.begin(); index != map.end(); index++)
		{
			if (index->second == hit.block.actor)
				return index->first;
		}

		return nullptr;
	}
}