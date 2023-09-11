#pragma once
#include <Scene/Component.h>
#include <Physics/WrapperPhysX.h>
#include <Generated/Collider.rfkh.h>


namespace Physics NAMESPACE()
{
	enum ENUM() ShapeType
	{
		Plane = 0,
		Box = 1,
		Sphere = 2,
		Capsule = 3
	};

	struct STRUCT() LICH_ENGINE ColliderShape
	{
		FIELD() ShapeType type = ShapeType::Box;
		FIELD() Vector3D scale = Vector3D::one * 0.5f;
		Physics_ColliderShape_GENERATED
	};

	class CLASS() LICH_ENGINE Collider : public SceneManagement::Component
	{
	private:
		FIELD() ColliderShape m_shape;
		ColliderShape m_oldShape;
		PhysicMaterial* m_pPhysicMaterial = nullptr;
		Shape* m_pSimulationShape = nullptr;

		FIELD() Vector3D m_offset = Vector3D::zero;
		FIELD() float m_staticFriction = 0.5f;
		FIELD() float m_dynamicFriction = 0.5f;
		FIELD() float m_restitution = 0.6f;

		FIELD() bool m_isTrigger = false;

		bool m_hasBeenModified = false;
		bool m_shapeHasBeenModified = false;

	public:
		Collider(SceneManagement::GameObject* gO)						{ componentType = SceneManagement::ComponentType::COLLIDER; p_pGameObject = gO; }
		~Collider();

		void SetShape(ColliderShape shape);
		void SetScale(Vector3D scale); // sphere will only use the first number of the vec for the radius, capsule the first for radius and second for height, for the plane it will be the normal
		void SetOffset(Vector3D offset);
		void SetTriggerState(bool isTriggered);
		void SetStaticFriction(float staticFriction);
		void SetDynamicFriction(float dynamicFriction);
		void SetRestitution(float restitution); // the value must be between 0 and 1
		void SetSimulationShape(Shape * simulationShape)				{ m_pSimulationShape = simulationShape; }
		void SetPhysicMaterialPointer(PhysicMaterial * physicMaterial)	{ m_pPhysicMaterial = physicMaterial; }

		void CopyCollider(Collider* collider);

		bool GetTriggerState()											{ return m_isTrigger; }
		bool GetRestitution()											{ return m_restitution; }

		float GetStaticFriction()										{ return m_staticFriction; }
		float GetDynamicFriction()										{ return m_dynamicFriction; }
		Vector3D GetOffset()											{ return m_offset; }
		ColliderShape GetShape()										{ return m_shape; }
		Vector3D GetScale()												{ return m_shape.scale; }
		ShapeType GetShapeType()										{ return m_shape.type; }
		Shape* GetSimulationShape()										{ return m_pSimulationShape; }
		PhysicMaterial* GetPhysicMaterialPointer()						{ return m_pPhysicMaterial; }

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		void UpdateSimulationShape(RigidActor * actor, PhysicsSimulator * physics, Vector3D gameObjectScale);

		Physics_Collider_GENERATED
	};
};
File_Collider_GENERATED