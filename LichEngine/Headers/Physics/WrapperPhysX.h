#pragma once
#undef min
#undef max
#include <physX/PxConfig.h>
#include <physX/PxPhysicsAPI.h>
#include <physX/PxSimulationEventCallback.h>
#include <Physics/Transform.h>


namespace Physics
{
	typedef physx::PxDefaultAllocator		DefaultAllocator;
	typedef physx::PxDefaultErrorCallback	DefaultErrorCallback;
	typedef physx::PxDefaultCpuDispatcher	DefaultCpuDispatcher;

	typedef physx::PxTolerancesScale		TolerancesScale;
	typedef physx::PxFoundation				Foundation;
	typedef physx::PxPhysics				PhysicsSimulator;
	typedef physx::PxScene					Scene;
	typedef physx::PxSceneDesc				SceneDesc;

	typedef physx::PxMaterial				PhysicMaterial;
	typedef physx::PxPvd					Pvd;
	typedef physx::PxPvdTransport			PvdTransport;
	typedef physx::PxPvdSceneClient			PvdSceneClient;
	typedef physx::PxPvdSceneFlag			PvdSceneFlag;
	typedef physx::PxPvdInstrumentationFlag PvdInstrumentationFlag;

	typedef physx::PxRigidBody				SimulationRigidBody;
	typedef physx::PxRigidStatic			RigidStatic;
	typedef physx::PxRigidDynamic			RigidDynamic;
	typedef physx::PxRigidActor				RigidActor;
	typedef physx::PxRigidBodyFlag			RigidBodyFlag;

	typedef physx::PxShape					Shape;
	typedef physx::PxShapeFlag				ShapeFlag;
	typedef physx::PxShapeFlags				ShapeFlags;

	typedef physx::PxActor					Actor;
	typedef physx::PxActorFlag				ActorFlag;
	typedef physx::PxActorType				ActorType;

	typedef physx::PxFilterFlags			FilterFlags;
	typedef physx::PxFilterObjectAttributes	FilterObjectAttributes;
	typedef physx::PxFilterData				FilterData;
	typedef physx::PxPairFlags				PairFlags;
	typedef physx::PxPairFlag				PairFlag;
	typedef physx::PxFilterFlag				FilterFlag;

	typedef physx::PxPlane					PlaneRepresentation;
	typedef physx::PxPlaneGeometry			PlaneGeometry;
	typedef physx::PxBoxGeometry			BoxGeometry;
	typedef physx::PxSphereGeometry			SphereGeometry;
	typedef physx::PxCapsuleGeometry		CapsuleGeometry;
	typedef physx::PxGeometry				Geometry;

	typedef physx::PxTransform				PhysicTransform;
	typedef physx::PxU32					SimulationU32;

	typedef physx::PxConstraintInfo			ConstraintInfo;
	typedef physx::PxContactPair			ContactPair;
	typedef physx::PxContactPairHeader		ContactPairHeader;
	typedef physx::PxTriggerPair			TriggerPair;

	typedef physx::PxRaycastBuffer			RaycastBuffer;
	typedef physx::PxSweepBuffer			SweepBuffer;
	
	Foundation* CreateFoundation(DefaultAllocator& allocator, DefaultErrorCallback& errorCallback);
	Pvd* CreatePvd(Foundation& foundation);
	PvdTransport* CreateDefaultPvdSocketTransport(const char* host, int port, unsigned int timeoutInMilliseconds);
	PhysicsSimulator* CreatePhysicsSimulator(Foundation* foundation, TolerancesScale tolerancesScale = TolerancesScale(), bool trackOutstandingAllocations = false, Pvd* pvd = NULL);
	DefaultCpuDispatcher* CreateDefaultCpuDispatcher(int threadsnumber);
	PhysicTransform GetPhysicTransformForPlane(Vector3D normal);

	Vector3D ToVector3D(physx::PxVec3 vec);
	Quaternion ToQuaternion(physx::PxQuat quat);
	Transform ToTransform(physx::PxTransform transform);

	physx::PxVec3 ToPxVec3(Vector3D vec);
	physx::PxVec3 ToNormalizedPxVec3(Vector3D vec);
	physx::PxVec3 ToPxVec3(float x, float y, float z);
	physx::PxQuat ToPxQuat(Quaternion quat);
	physx::PxTransform ToPxTransform(Transform transform);
	physx::PxTransform ToPxTransform(Vector3D vec);

	class PhysicsManager;

	class SimulationEventCallback: public physx::PxSimulationEventCallback
	{
	private:
		
	public:
		PhysicsManager* pPhysicsManager;

		void onConstraintBreak(ConstraintInfo* constraints, SimulationU32 count) override;														//This is called when a breakable constraint breaks.
		void onWake(Actor** actors, SimulationU32 count) override;																				//This is called with the actors which have just been woken up.
		void onSleep(Actor** actors, SimulationU32 count) override;																				//This is called with the actors which have just been put to sleep.
		void onContact(const ContactPairHeader& pairHeader, const ContactPair* pairs, SimulationU32 nbPairs) override;							//This is called when certain contact events occur.
		void onTrigger(TriggerPair* pairs, SimulationU32 count) override;																		//This is called with the current trigger pair events.
		void onAdvance(const SimulationRigidBody* const* bodyBuffer, const PhysicTransform* poseBuffer, const SimulationU32 count) override;	//Provides early access to the new pose of moving rigid bodies.
	};

	static FilterFlags contactReportFilterShader(FilterObjectAttributes attributes0, FilterData filterData0,
		FilterObjectAttributes attributes1, FilterData filterData1,
		PairFlags& pairFlags, const void* constantBlock, SimulationU32 constantBlockSize)
	{
		PX_UNUSED(attributes0);
		PX_UNUSED(attributes1);
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);
		PX_UNUSED(constantBlockSize);
		PX_UNUSED(constantBlock);

		// all initial and persisting reports for everything, with per-point data
		pairFlags = PairFlag::eSOLVE_CONTACT | PairFlag::eDETECT_DISCRETE_CONTACT
			| PairFlag::eNOTIFY_TOUCH_FOUND
			| PairFlag::eNOTIFY_TOUCH_PERSISTS
			| PairFlag::eNOTIFY_TOUCH_LOST
			| PairFlag::eNOTIFY_CONTACT_POINTS;
		return FilterFlag::eDEFAULT;
	}
}

class PxAllocatorCallback
{
public:
	virtual ~PxAllocatorCallback() {}
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) = 0;
	virtual void deallocate(void* ptr) = 0;
};

class UserErrorCallback : public physx::PxErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		std::cout << "Error code" << code << " : " << message << ", file : " << file << ", line : " << line << std::endl;
	}
};