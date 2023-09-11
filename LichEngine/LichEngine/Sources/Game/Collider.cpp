#include <pch.h>
#include <Debugs/Log.h>

#include <Generated/Collider.rfks.h>

Physics::Collider::~Collider()
{
	m_pPhysicMaterial = nullptr;
	m_pSimulationShape = nullptr;
}

void Physics::Collider::SetShape(ColliderShape shape)
{
	m_oldShape = m_shape;
	m_shape = shape;
	
	m_shapeHasBeenModified = true;
	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetScale(Vector3D scale)
{
	m_shape.scale = scale;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetOffset(Vector3D offset)
{
	m_offset = offset;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetTriggerState(bool isTriggered)
{
	m_isTrigger = isTriggered;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetStaticFriction(float staticFriction)
{
	m_staticFriction = staticFriction;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetDynamicFriction(float dynamicFriction)
{
	m_dynamicFriction = dynamicFriction;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::SetRestitution(float restitution)
{
	if (restitution > 1.f || restitution < 0.f)
	{
		LWARNING("Restitution value must be between 0 and 1.");
		return;
	}

	m_restitution = restitution;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::Collider::Update(Physics::Transform& transform)
{

}

void Physics::Collider::LoadData(std::string data)
{
	const int sizeData = 7;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	ColliderShape shape;
	shape.type = (ShapeType)std::stoi(dataCut[0]);
	shape.scale = Vector3D::GetFromString(dataCut[1]);
	SetShape(shape);
	m_offset = Vector3D::GetFromString(dataCut[2]);
	m_staticFriction = std::stof(dataCut[3]);
	m_dynamicFriction = std::stof(dataCut[4]);
	m_restitution = std::stof(dataCut[5]);
	m_isTrigger = std::stoi(dataCut[6]);
}

std::string Physics::Collider::SaveData()
{
	std::string content = "~6,";
	content += std::to_string((int)m_shape.type) + ',';
	content += m_shape.scale.ToString() + ',';
	content += m_offset.ToString() + ',';
	content += std::to_string(m_staticFriction) + ',';
	content += std::to_string(m_dynamicFriction) + ',';
	content += std::to_string(m_restitution) + ',';
	content += std::to_string(m_isTrigger) + ',';

	return content;
}

void Physics::Collider::ApplyEditorChange()
{
	m_hasBeenModified = true;
	m_shapeHasBeenModified = true;
}

void Physics::Collider::UpdateSimulationShape(RigidActor* actor, PhysicsSimulator* physics, Vector3D gameObjectScale)
{
	if (!isEnable)
	{
		actor->setActorFlag(ActorFlag::eDISABLE_SIMULATION, true);
		return;
	}
	actor->setActorFlag(ActorFlag::eDISABLE_SIMULATION, false);

	PhysicTransform transform = actor->getGlobalPose();
	transform.p += ToPxVec3(m_offset);
	actor->setGlobalPose(transform);

	bool isStatic = false;
	ActorType::Enum type = actor->getType();
	if (type == ActorType::eRIGID_STATIC)
	{
		isStatic = true;
	}

	if (m_hasBeenModified)
	{
		if (gameObjectScale.x <= 0.f) gameObjectScale.x = 1.f;
		if (gameObjectScale.y <= 0.f) gameObjectScale.y = 1.f;
		if (gameObjectScale.z <= 0.f) gameObjectScale.z = 1.f;

		actor->detachShape(*m_pSimulationShape);

		Geometry* geometry = nullptr;

		if (m_shapeHasBeenModified)
		{
			if (!isStatic && m_shape.type == ShapeType::Plane)
			{
				LWARNING("A non-static collider can't be a plane.");
				m_shape = m_oldShape;
			}
			if (m_shape.scale.x <= 0.f) m_shape.scale.x = 0.5f;

			PhysicTransform tr;
			switch (m_shape.type)
			{
			case ShapeType::Plane:
				m_shape.scale = Vector3D(0, 1, 0);
				geometry = new PlaneGeometry();
				tr = GetPhysicTransformForPlane(Vector3D(0, 1, 0));
				tr.p = actor->getGlobalPose().p;
				actor->setGlobalPose(tr);
				break;
			case ShapeType::Box:
				if (m_shape.scale.y <= 0.f || m_shape.scale.z <= 0.f)
				{
					m_shape.scale.y = m_shape.scale.x;
					m_shape.scale.z = m_shape.scale.x;
				}
				geometry = new BoxGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y, m_shape.scale.z * gameObjectScale.z);
				break;
			case ShapeType::Sphere:
				geometry = new SphereGeometry(m_shape.scale.x * gameObjectScale.x);
				break;
			case ShapeType::Capsule:
				if (m_shape.scale.y <= 0.f) m_shape.scale.y = 0.5f;
				geometry = new CapsuleGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y);
				break;
			default:
				geometry = new BoxGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y, m_shape.scale.z * gameObjectScale.z);
				break;
			}

			m_pSimulationShape = physics->createShape(*geometry, *m_pPhysicMaterial, true, ShapeFlag::eVISUALIZATION | ShapeFlag::eSCENE_QUERY_SHAPE);
		}
		else
		{
			m_pSimulationShape->setMaterials(&m_pPhysicMaterial, 1);

			switch (m_shape.type)
			{
			case ShapeType::Plane:
				m_pSimulationShape->setGeometry(PlaneGeometry());
				break;
			case ShapeType::Box:
				m_pSimulationShape->setGeometry(BoxGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y, m_shape.scale.z * gameObjectScale.z));
				break;
			case ShapeType::Sphere:
				m_pSimulationShape->setGeometry(SphereGeometry(m_shape.scale.x * gameObjectScale.x));
				break;
			case ShapeType::Capsule:
				m_pSimulationShape->setGeometry(CapsuleGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y));
				break;
			default:
				m_pSimulationShape->setGeometry(BoxGeometry(m_shape.scale.x * gameObjectScale.x, m_shape.scale.y * gameObjectScale.y, m_shape.scale.z * gameObjectScale.z));
				break;
			}
		}

		m_pPhysicMaterial->setStaticFriction(m_staticFriction);
		m_pPhysicMaterial->setDynamicFriction(m_dynamicFriction);
		m_pPhysicMaterial->setRestitution(m_restitution);

		if (m_isTrigger)
		{
			m_pSimulationShape->setFlag(ShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
			m_pSimulationShape->setFlag(ShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
		}
		else
		{
			m_pSimulationShape->setFlag(ShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
			m_pSimulationShape->setFlag(ShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
		}

		actor->attachShape(*m_pSimulationShape);
		m_hasBeenModified = false;
	}
}

void Physics::Collider::CopyCollider(Collider* collider)
{
	m_shape = collider->m_shape;
	m_offset = collider->m_offset;
	m_restitution = collider->m_restitution;
	m_isTrigger = collider->m_isTrigger;
	m_staticFriction = collider->m_staticFriction;
	m_dynamicFriction = collider->m_dynamicFriction;
}
