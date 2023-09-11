#pragma once
#include <map>

#include <Physics/WrapperPhysX.h>
#include <Scene/Scene.h>

namespace Physics
{
	struct ColliderShape;

	class LICH_ENGINE PhysicsManager
	{
	private:
		DefaultAllocator			m_DefaultAllocatorCallback;
		DefaultErrorCallback		m_DefaultErrorCallback;
		DefaultCpuDispatcher*		m_pDispatcher					= NULL;
		TolerancesScale				m_ToleranceScale;
		SimulationEventCallback*	m_pSimulationEventCallback		= NULL;

		Foundation*					m_pFoundation					= NULL;
		PhysicsSimulator*			m_pPhysics						= NULL;
		Scene*						m_pScene						= NULL;

		Pvd*						m_pPvd							= NULL;

		std::map <SceneManagement::GameObject*, RigidActor*> m_actors;

		SceneManagement::Scene* m_pGameScene;

		Geometry* CreateActorGeometry(const ColliderShape& shape);
	public:
		void Init(SceneManagement::Scene* scenePtr);
		void AddPhysicsActor(SceneManagement::GameObject* gO);						
		void DeletePhysicsActor(SceneManagement::GameObject* gO);
		void RunSimulation();
		void Release();

		Scene* GetSimulationScene() { return m_pScene; }
		std::map <SceneManagement::GameObject*, RigidActor*> GetSimulationActors() { return m_actors; }
	};

	SceneManagement::GameObject* Raycast(PhysicsManager* manager, const Vector3D& origin, const Vector3D& direction, const float range);
	SceneManagement::GameObject* Boxcast(PhysicsManager* manager, const Vector3D& halfExtent, const Vector3D& origin, const Vector3D& direction, const float range);
}