#include <pch.h>
#include <Generated/RigidBody.rfks.h>


void Physics::RigidBody::Update(Physics::Transform& transform)
{
}

void Physics::RigidBody::LoadData(std::string data)
{
	const int sizeData = 13;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	m_isKinematic = std::stoi(dataCut[0]);
	m_useGravity = std::stoi(dataCut[1]);
	m_mass = std::stof(dataCut[2]);
	m_drag = std::stof(dataCut[3]);
	m_angularDrag = std::stof(dataCut[4]);
	constraints.PositionX = std::stoi(dataCut[5]);
	constraints.PositionY = std::stoi(dataCut[6]);
	constraints.PositionZ = std::stoi(dataCut[7]);
	constraints.RotationX = std::stoi(dataCut[8]);
	constraints.RotationY = std::stoi(dataCut[9]);
	constraints.RotationZ = std::stoi(dataCut[10]);

	m_interpolationMode = std::stoi(dataCut[11]);
	m_collisionDetectionMode = std::stoi(dataCut[12]);
	m_hasBeenModified = true;
}

std::string Physics::RigidBody::SaveData()
{
	std::string content = "~5,";
	content += std::to_string(m_isKinematic) + ',';
	content += std::to_string(m_useGravity) + ',';
	content += std::to_string(m_mass) + ',';
	content += std::to_string(m_drag) + ',';
	content += std::to_string(m_angularDrag) + ',';
	content += std::to_string(constraints.PositionX) + ',';
	content += std::to_string(constraints.PositionY) + ',';
	content += std::to_string(constraints.PositionZ) + ',';
	content += std::to_string(constraints.RotationX) + ',';
	content += std::to_string(constraints.RotationY) + ',';
	content += std::to_string(constraints.RotationZ) + ',';

	content += std::to_string(m_interpolationMode) + ',';
	content += std::to_string(m_collisionDetectionMode) + ',';

	return content;
}

void Physics::RigidBody::ApplyEditorChange()
{
	m_hasBeenModified = true;
}

void Physics::RigidBody::UpdateRigidActor(RigidDynamic* rd)
{
	if (m_hasBeenModified)
	{
		rd->setRigidBodyFlag(RigidBodyFlag::eKINEMATIC, m_isKinematic);
		if (!m_isKinematic)
		{
			rd->setActorFlag(ActorFlag::eDISABLE_GRAVITY, !m_useGravity);

			rd->setMass(m_mass);

			rd->setLinearVelocity(ToPxVec3(m_velocity));
			rd->setAngularVelocity(ToPxVec3(m_angularVelocity));

			rd->setLinearDamping(m_drag);
			rd->setAngularDamping(m_angularDrag);

			rd->setMassSpaceInertiaTensor(ToPxVec3(m_inertiaTensor));

			rd->setCMassLocalPose(PhysicTransform(ToPxVec3(m_localCenterofMass)));

			if (rd->isSleeping() && !m_isSleeping)
				rd->wakeUp();
			else if (!rd->isSleeping() && m_isSleeping)
				rd->putToSleep();
		}

		m_hasBeenModified = false;
	}

	for (int i = 0; i < m_forcesAdded.size(); i++)
	{
		switch ((ForceMode)m_forcesAdded[i].w)
		{
		case FORCE:
			rd->addForce(ToPxVec3(Vector3D(m_forcesAdded[i].x, m_forcesAdded[i].y, m_forcesAdded[i].z)), physx::PxForceMode::eFORCE);
			break;
		case IMPULSE:
			rd->addForce(ToPxVec3(Vector3D(m_forcesAdded[i].x, m_forcesAdded[i].y, m_forcesAdded[i].z)), physx::PxForceMode::eIMPULSE);
			break;
		case VELOCITY_CHANGE:
			rd->addForce(ToPxVec3(Vector3D(m_forcesAdded[i].x, m_forcesAdded[i].y, m_forcesAdded[i].z)), physx::PxForceMode::eVELOCITY_CHANGE);
			break;
		default:
			break;
		}
	}
	m_forcesAdded.clear();

	for (int i = 0; i < m_torquesAdded.size(); i++)
	{
		switch ((ForceMode)m_torquesAdded[i].w)
		{
		case FORCE:
			rd->addTorque(ToPxVec3(Vector3D(m_torquesAdded[i].x, m_torquesAdded[i].y, m_torquesAdded[i].z)), physx::PxForceMode::eFORCE);
			break;
		case IMPULSE:
			rd->addTorque(ToPxVec3(Vector3D(m_torquesAdded[i].x, m_torquesAdded[i].y, m_torquesAdded[i].z)), physx::PxForceMode::eIMPULSE);
			break;
		case VELOCITY_CHANGE:
			rd->addTorque(ToPxVec3(Vector3D(m_torquesAdded[i].x, m_torquesAdded[i].y, m_torquesAdded[i].z)), physx::PxForceMode::eVELOCITY_CHANGE);
			break;
		default:
			break;
		}
	}
	m_torquesAdded.clear();
}

void Physics::RigidBody::UpdateFromRigidActor(RigidDynamic* rd)
{
	if (!m_isKinematic)
	{
		m_velocity = ToVector3D(rd->getLinearVelocity());
		m_speed = m_velocity.Magnitude();
		m_angularVelocity = ToVector3D(rd->getAngularVelocity());

		m_inertiaTensor = ToVector3D(rd->getMassSpaceInertiaTensor());
		m_localCenterofMass = ToVector3D(rd->getCMassLocalPose().p);

		m_isSleeping = rd->isSleeping();
	}
}

void Physics::RigidBody::AddForce(const Vector3D& force, const ForceMode mode)
{
  	m_forcesAdded.push_back(Vector4D(force, (float)mode));
}

void Physics::RigidBody::AddTorque(const Vector3D& force, const ForceMode mode)
{
	m_torquesAdded.push_back(Vector4D(force, (float)mode));
}

void Physics::RigidBody::SetSleepingState(bool isSleeping) 
{
	m_isSleeping = isSleeping;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetKinematicState(bool isKinematic) 
{
	m_isKinematic = isKinematic;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetGravityUse(bool useGravity) 
{
	m_useGravity = useGravity;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetPositionConstraints(bool posX, bool posY, bool posZ) 
{
	constraints.PositionX = posX;
	constraints.PositionY = posY;
	constraints.PositionZ = posZ;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetPositionXConstraint(bool posX)
{
	constraints.PositionX = posX;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetPositionYConstraint(bool posY)
{
	constraints.PositionY = posY;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetPositionZConstraint(bool posZ)
{
	constraints.PositionZ = posZ;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetRotationConstraints(bool axisX, bool axisY, bool axisZ)
{
	constraints.RotationX = axisX;
	constraints.RotationY = axisY;
	constraints.RotationZ = axisZ;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetRotationXConstraint(bool axisX)
{
	constraints.RotationX = axisX;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetRotationYConstraint(bool axisY)
{
	constraints.RotationY = axisY;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetRotationZConstraint(bool axisZ)
{
	constraints.RotationZ = axisZ;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetMass(float newMass)
{
	m_mass = newMass;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetLinearDragCoefficient(float newDrag)
{
	m_drag = newDrag;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetAngularDragCoefficient(float newAngularDrag)
{
	m_angularDrag = newAngularDrag;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetSpeed(float newSpeed)
{
	m_speed = newSpeed;
	m_velocity = Vector3D::Normalize(m_velocity) * m_speed;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetVelocity(Vector3D newVelocity)
{
	m_velocity = newVelocity;
	m_speed = m_velocity.Magnitude();

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetAngularVelocity(Vector3D newVelocity)
{
	m_angularVelocity = newVelocity;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetInertiaTensor(Vector3D newTensor)
{
	m_inertiaTensor = newTensor;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetInertiaTensorRotation(Vector3D newRotation)
{
	m_inertiaTensorRotation = newRotation;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetLocalCenterOfMass(Vector3D newCenter)
{
	m_localCenterofMass = newCenter;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}

void Physics::RigidBody::SetGlobalCenterOfMass(Vector3D newCenter)
{
	m_globalCenterofMass = newCenter;

	if (!m_hasBeenModified)
		m_hasBeenModified = true;
}
