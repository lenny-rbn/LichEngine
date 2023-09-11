#pragma once
#include <Scene/Component.h>
#include <Physics/WrapperPhysX.h>
#include <Reflexion/TitleReflectionProperty.h>
#include <Generated/RigidBody.rfkh.h>


namespace Physics NAMESPACE()
{
	enum ENUM() ForceMode
	{
		FORCE,
		IMPULSE,
		VELOCITY_CHANGE,
	};

	struct STRUCT() Constraints
	{
		FIELD(TitleReflectionProperty("Constraints")) bool PositionX = false;
		FIELD(SameLineProperty()) bool PositionY = false;
		FIELD(SameLineProperty())bool PositionZ = false;
		FIELD()bool RotationX = false;
		FIELD(SameLineProperty())bool RotationY = false;
		FIELD(SameLineProperty())bool RotationZ = false;

		Physics_Constraints_GENERATED
	};

	class CLASS() LICH_ENGINE RigidBody : public SceneManagement::Component
	{
	private:
		//Infos
		FIELD() float m_speed						= 0.f;
		FIELD() Vector3D m_velocity					= Vector3D::zero;
		FIELD() Vector3D m_angularVelocity			= Vector3D::zero;
		FIELD() Vector3D m_inertiaTensor			= Vector3D::one;
		FIELD() Vector3D m_inertiaTensorRotation	= Vector3D::zero;
		FIELD() Vector3D m_localCenterofMass		= Vector3D::zero; //center of mass relative to the actor
		FIELD() Vector3D m_globalCenterofMass		= Vector3D::zero;

		FIELD() bool m_isSleeping					= false;
		FIELD() bool m_isKinematic					= false;
		FIELD() bool m_useGravity					= true;

		FIELD() float m_mass						= 1.f;
		FIELD() float m_drag						= 0.f;		//linear drag coefficient. 0 means no damping.
		FIELD() float m_angularDrag					= 0.05f;	//angular drag coefficient. 0 means no damping.

		//Constraints
		FIELD() Constraints constraints;

		//Unity RigidBody got theses but i don't know what are their purposes or how to implemente it :: TODO 
		int m_interpolationMode				= 0;	//must tranform int into enum
		int m_collisionDetectionMode		= 0;	//same

		bool m_hasBeenModified = false;

		std::vector<Vector4D> m_forcesAdded;
		std::vector<Vector4D> m_torquesAdded;
		
	public:
		RigidBody(SceneManagement::GameObject* gO) { componentType = SceneManagement::ComponentType::RIGIDBODY; p_pGameObject = gO; }
		~RigidBody() { };

		void Update(Physics::Transform& transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		void UpdateRigidActor(RigidDynamic* actor);
		void UpdateFromRigidActor(RigidDynamic* actor);

		void AddForce(const Vector3D& force, const ForceMode mode);
		void AddTorque(const Vector3D& force, const ForceMode mode);

		void SetSleepingState(bool isSleeping);
		void SetKinematicState(bool isKinematic);
		void SetGravityUse(bool useGravity);
		void SetPositionConstraints(bool posX, bool posY, bool posZ);
		void SetPositionXConstraint(bool posX);
		void SetPositionYConstraint(bool posY);
		void SetPositionZConstraint(bool posZ);
		void SetRotationConstraints(bool axisX, bool axisY, bool axisZ);
		void SetRotationXConstraint(bool axisX);
		void SetRotationYConstraint(bool axisY);
		void SetRotationZConstraint(bool axisZ);
		void SetMass(float newMass);
		void SetLinearDragCoefficient(float newDrag);
		void SetAngularDragCoefficient(float newAngularDrag);
		void SetSpeed(float newSpeed);
		void SetVelocity(Vector3D newVelocity);
		void SetAngularVelocity(Vector3D newVelocity);
		void SetInertiaTensor(Vector3D newTensor);
		void SetInertiaTensorRotation(Vector3D newRotation);
		void SetLocalCenterOfMass(Vector3D newCenter);
		void SetGlobalCenterOfMass(Vector3D newCenter);

		bool GetSleepingState()				{ return m_isSleeping; }
		bool GetKinematicState()			{ return m_isKinematic; }
		bool GetGravityUse()				{ return m_useGravity; }
		bool GetPositionXConstraint()		{ return constraints.PositionX; }
		bool GetPositionYConstraint()		{ return constraints.PositionY; }
		bool GetPositionZConstraint()		{ return constraints.PositionZ; }
		bool GetRotationXConstraint()		{ return constraints.RotationX; }
		bool GetRotationYConstraint()		{ return constraints.RotationY; }
		bool GetRotationZConstraint()		{ return constraints.RotationZ; }
		Constraints& GetConstraints()		{ return constraints; }
		float GetMass()						{ return m_mass; }
		float GetLinearDragCoefficient()	{ return m_drag; }
		float GetAngularDragCoefficient()	{ return m_angularDrag; }
		float GetSpeed()					{ return m_speed; }
		Vector3D GetVelocity()				{ return m_velocity; }
		Vector3D GetAngularVelocity()		{ return m_angularVelocity; }
		Vector3D GetInertiaTensor()			{ return m_inertiaTensor; }
		Vector3D GetInertiaTensorRotation() { return m_inertiaTensorRotation; }
		Vector3D GetLocalCenterOfMass()		{ return m_localCenterofMass; }
		Vector3D GetGlobalCenterOfMass()	{ return m_globalCenterofMass; }

		Physics_RigidBody_GENERATED
	};
};
File_RigidBody_GENERATED