#include <pch.h>
#include <Physics/PhysicsManager.h>
#include <Physics/WrapperPhysX.h>
#include <Scene/GameObject.h>


Physics::Foundation* Physics::CreateFoundation(DefaultAllocator& allocator, DefaultErrorCallback& errorCallback)
{
	return PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
}

Physics::Pvd* Physics::CreatePvd(Foundation& foundation)
{
	return PxCreatePvd(foundation);
}

Physics::PvdTransport* Physics::CreateDefaultPvdSocketTransport(const char* host, int port, unsigned int timeoutInMilliseconds)
{
	return physx::PxDefaultPvdSocketTransportCreate(host, port, timeoutInMilliseconds);
}

Physics::PhysicsSimulator* Physics::CreatePhysicsSimulator(Foundation* foundation, TolerancesScale tolerancesScale, bool trackOutstandingAllocations, Pvd* pvd)
{
	return PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, tolerancesScale, trackOutstandingAllocations, pvd);;
}

Physics::DefaultCpuDispatcher* Physics::CreateDefaultCpuDispatcher(int threadsnumber)
{
	return physx::PxDefaultCpuDispatcherCreate(threadsnumber);
}

Physics::PhysicTransform Physics::GetPhysicTransformForPlane(Vector3D normal)
{
	return physx::PxTransformFromPlaneEquation(PlaneRepresentation(ToPxVec3(normal), 0.f));
}

Vector3D Physics::ToVector3D(physx::PxVec3 vec)
{
	return Vector3D(vec.x, vec.y, vec.z);
}

Quaternion Physics::ToQuaternion(physx::PxQuat quat)
{
	return Quaternion(quat.w, quat.x, quat.y, quat.z);
}

Physics::Transform Physics::ToTransform(physx::PxTransform transform)
{
	return Transform(nullptr, ToVector3D(transform.p), ToQuaternion(transform.q));
}

physx::PxVec3 Physics::ToPxVec3(Vector3D vec)
{
	return physx::PxVec3(vec.x, vec.y, vec.z);
}

physx::PxVec3 Physics::ToNormalizedPxVec3(Vector3D vec)
{
	physx::PxVec3 pxVec(vec.x, vec.y, vec.z);
	pxVec.normalize();
	return pxVec;
}

physx::PxVec3 Physics::ToPxVec3(float x, float y, float z)
{
	return physx::PxVec3(x, y, z);
}

physx::PxQuat Physics::ToPxQuat(Quaternion quat)
{
	return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

physx::PxTransform Physics::ToPxTransform(Transform transform)
{
	physx::PxQuat quat = ToPxQuat(transform.GetGlobalRotationInQuaternion());
	return physx::PxTransform(ToPxVec3(transform.GetGlobalPosition()), quat);
}

physx::PxTransform Physics::ToPxTransform(Vector3D vec)
{
	return physx::PxTransform(ToPxVec3(vec));
}

void Physics::SimulationEventCallback::onConstraintBreak(ConstraintInfo* constraints, SimulationU32 count)
{
	//printf("onConstraintBreak\n");
}

void Physics::SimulationEventCallback::onWake(Actor** actors, SimulationU32 count)
{
	//printf("onWake\n");
}

void Physics::SimulationEventCallback::onSleep(Actor** actors, SimulationU32 count)
{
	//printf("onSleep\n");
}

void Physics::SimulationEventCallback::onContact(const ContactPairHeader& pairHeader, const ContactPair* pairs, SimulationU32 nbPairs)
{
	std::map <SceneManagement::GameObject*, RigidActor*> actors = pPhysicsManager->GetSimulationActors();
	
	SceneManagement::GameObject* go1 = nullptr;
	SceneManagement::GameObject* go2 = nullptr;
	
	for (std::map<SceneManagement::GameObject*, RigidActor*>::iterator index = actors.begin(); index != actors.end(); index++)
	{
		if (index->second == pairHeader.actors[0])
			go1 = index->first;

		if (index->second == pairHeader.actors[1])
			go2 = index->first;
	}

	if (pairs->events.isSet(PairFlag::eNOTIFY_TOUCH_FOUND))
	{
		go1->OnCollisionEnter(go2);
		go2->OnCollisionEnter(go1);
	}
	if (pairs->events.isSet(PairFlag::eNOTIFY_TOUCH_PERSISTS))
	{
		go1->OnCollisionStay(go2);
		go2->OnCollisionStay(go1);
	}
	if (pairs->events.isSet(PairFlag::eNOTIFY_TOUCH_LOST))
	{
		go1->OnCollisionExit(go2);
		go2->OnCollisionExit(go1);
	}
}

void Physics::SimulationEventCallback::onTrigger(TriggerPair* pairs, SimulationU32 count)
{
	std::map <SceneManagement::GameObject*, RigidActor*> actors = pPhysicsManager->GetSimulationActors();

	SceneManagement::GameObject* go1 = nullptr;
	SceneManagement::GameObject* go2 = nullptr;

	for (std::map<SceneManagement::GameObject*, RigidActor*>::iterator index = actors.begin(); index != actors.end(); index++)
	{
		if (index->second == pairs->triggerActor)
			go1 = index->first;

		if (index->second == pairs->otherActor)
			go2 = index->first;
	}

	if (pairs->status == PairFlag::eNOTIFY_TOUCH_FOUND)
		go1->OnTriggerEnter(go2);

	if (pairs->status == PairFlag::eNOTIFY_TOUCH_LOST)
		go1->OnTriggerExit(go2);
}

void Physics::SimulationEventCallback::onAdvance(const SimulationRigidBody* const* bodyBuffer, const PhysicTransform* poseBuffer, const SimulationU32 count)
{
	//printf("onAdvance\n");
}
