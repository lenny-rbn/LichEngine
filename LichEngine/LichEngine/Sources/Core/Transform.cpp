#include <pch.h>
#include <Scene/GameObject.h>
#include <Physics/Collider.h>
#include <Physics/RigidBody.h>

#include <Generated/Transform.rfks.h>

Physics::Transform::Transform()
{
	m_pCurrentObject = nullptr;
	m_localPosition = Vector3D::zero;
	m_localRotation = Vector3D::zero;
	m_localScale = Vector3D::one;
	m_globalRot = m_localRot = Quaternion(1,0,0,0);
	m_pParent = nullptr;

	CalculateModelMatrix();
}

Physics::Transform::Transform(SceneManagement::GameObject* obj)
{
	m_pCurrentObject = obj;
	m_globalPosition = m_localPosition = Vector3D::zero;
	m_globalRotation = m_localRotation = Vector3D::zero;
	m_globalScale = m_localScale = Vector3D::one;
	m_globalRot = m_localRot = Quaternion(1,0,0,0);
	m_pParent = nullptr;

	CalculateModelMatrix();
	m_globalModelMatrix = m_localModelMatrix;
}

Physics::Transform::Transform(SceneManagement::GameObject* obj,Vector3D pos)
{
	m_pCurrentObject = obj;
	m_globalPosition = m_localPosition = pos;
	m_globalRotation = m_localRotation = Vector3D::zero;
	m_globalScale = m_localScale = Vector3D::one;
	m_globalRot = m_localRot = Quaternion(1,0,0,0);
	m_pParent = nullptr;

	CalculateModelMatrix();
	m_globalModelMatrix = m_localModelMatrix;
}

Physics::Transform::Transform(SceneManagement::GameObject* obj,Vector3D pos,Quaternion rot)
{
	m_pCurrentObject = obj;
	m_globalPosition = m_localPosition = pos;
	m_globalRotation = m_localRotation = Quaternion::GetEulerAnglesFromQuaternion(rot);
	m_globalScale = m_localScale = Vector3D::one;
	m_globalRot = m_localRot = rot;
	m_pParent = nullptr;

	CalculateModelMatrix();
	m_globalModelMatrix = m_localModelMatrix;
}

Physics::Transform::Transform(SceneManagement::GameObject* obj,Vector3D pos,Vector3D rot)
{
	m_pCurrentObject = obj;
	m_globalPosition = m_localPosition = pos;
	m_globalRotation = m_localRotation = rot;
	m_globalScale = m_localScale = Vector3D::one;
	m_globalRot = m_localRot = Quaternion::GetQuaternionFromEulerAngles(rot);
	m_pParent = nullptr;

	CalculateModelMatrix();
	m_globalModelMatrix = m_localModelMatrix;
}

Physics::Transform::Transform(SceneManagement::GameObject* obj,Vector3D pos,Vector3D rot,Vector3D sca)
{
	m_pCurrentObject = obj;
	m_globalPosition = m_localPosition = pos;
	m_globalRotation = m_localRotation = rot;
	m_globalScale = m_localScale = sca;
	m_globalRot = m_localRot = Quaternion::GetQuaternionFromEulerAngles(rot);
	m_pParent = nullptr;

	CalculateModelMatrix();
	m_globalModelMatrix = m_localModelMatrix;
}

Physics::Transform::~Transform()
{
	m_pParent = nullptr;
	m_pCurrentObject = nullptr;
	m_children.clear();
	m_localModifications.clear();
	m_globalModifications.clear();
}

void Physics::Transform::PostPhysicUpdate()
{
	if (m_postPhysicUpdated) return;

	if (HasParent() && !m_pParent->m_postPhysicUpdated)
	{
		m_pParent->PostPhysicUpdate();
		return;
	}

	if (m_modifiedByPhysics)
		UpdateAllLocalVariables();
	else
		UpdateAllGlobalVariables();
	
	m_modifiedByPhysics = false;
	m_postPhysicUpdated = true;

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->PostPhysicUpdate();
	}
}

void Physics::Transform::CalculateTemp()
{
	if (m_localsHasBeenModified)
	{
		ApplyLocalModifications();
		UpdateAllGlobalVariables();
	}
	if (m_globalsHasBeenModified)
	{
		ApplyGlobalModifications();
		UpdateAllLocalVariables();
	}
	m_localsHasBeenModified = false;
	m_globalsHasBeenModified = false;
}

void Physics::Transform::Update(CameraRender::Camera* cam,bool isParentUpdated = false, bool isPlaying = false)
{
	if (m_updated == true) return;

	if (HasParent())
	{
		if (!m_pParent->m_updated)
		{
			m_pParent->Update(cam);
			return;
		}
	}

	m_isPlaying = isPlaying;
	bool modified = false;

	if (m_localsHasBeenModified || isParentUpdated)
	{
		ApplyLocalModifications();
		UpdateAllGlobalVariables();
		modified = true;
	}

	if (m_globalsHasBeenModified)
	{
		ApplyGlobalModifications();
		UpdateAllLocalVariables();
		modified = true;
	}

	m_localModifications.clear();
	m_globalModifications.clear();
	m_localsHasBeenModified = false;
	m_globalsHasBeenModified = false;
	m_postPhysicUpdated = false;

	CalculateMVP(cam->GetVPMatrix());

	m_pCurrentObject->Update(cam,isPlaying);
	m_updated = true;

	for (int i = 0; i < m_children.size(); i++)
		m_children[i]->Update(cam,modified, isPlaying);
}

std::string Physics::Transform::SaveData()
{
	std::string content = "~T,";
	content += m_localPosition.ToString() + ',';
	content += m_localRotation.ToString() + ',';
	content += m_localScale.ToString() + ',';
	if (m_pParent != nullptr)
		content += (m_pParent->GetGameObject()->GetName()) + ',';
	else content += "none,";
	return content;
}

void Physics::Transform::LoadData(std::string content)
{
	const int sizeData = 4;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = content.find(',',baseIndex + 1);
		dataCut[i] = content.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}
	
	if (dataCut[3] != "none")
		AddParent(GetGameObject()->GetSceneGraph()->GetGameObject(dataCut[3].c_str())->GetTransform());

	m_localPosition = m_globalPosition = Vector3D::GetFromString(dataCut[0]);
	m_localRotation = m_globalRotation = Vector3D::GetFromString(dataCut[1]);
	m_localRot = m_globalRot = Quaternion::GetQuaternionFromEulerAngles(m_localRotation);
	m_localScale = m_globalScale = Vector3D::GetFromString(dataCut[2]);
}

void Physics::Transform::UpdateFromPhysics(const Transform& transform)
{
	Constraints constraints;
	bool hasConstraints = false;
	if (m_pCurrentObject->HasComponent<RigidBody>())
	{
		constraints = m_pCurrentObject->GetComponent<RigidBody>()->GetConstraints();

		if (constraints.PositionX || constraints.PositionY || constraints.PositionZ || constraints.RotationX || constraints.RotationY || constraints.RotationZ)
			hasConstraints = true;
	}

	if (hasConstraints)
	{
		Vector3D pos = transform.m_globalPosition;
		Vector3D rot = Quaternion::GetEulerAnglesFromQuaternion(transform.m_globalRot);
		if (m_pCurrentObject->GetComponent<Collider>()->GetShapeType() == Physics::ShapeType::Capsule)
		{
			rot.z -= 90.f;
		}

		if (constraints.PositionX)
			pos.x = m_globalPosition.x;
		if (constraints.PositionY)
			pos.y = m_globalPosition.y;
		if (constraints.PositionZ)
			pos.z = m_globalPosition.z;

		if (constraints.RotationX)
			rot.x = m_globalRotation.x;
		if (constraints.RotationY)
			rot.y = m_globalRotation.y;
		if (constraints.RotationZ)
			rot.z = m_globalRotation.z;

		m_globalPosition = pos;
		m_globalRot = Quaternion::GetQuaternionFromEulerAngles(rot);
		m_globalRotation = rot;
	}
	else
	{
		m_globalPosition = transform.m_globalPosition;
		m_globalRot = transform.m_globalRot;
		m_globalRotation = Quaternion::GetEulerAnglesFromQuaternion(m_globalRot);

		if (m_pCurrentObject->GetComponent<Collider>()->GetShapeType() == Physics::ShapeType::Capsule)
		{
			m_globalRotation.z -= 90.f;
			m_globalRot = Quaternion::GetQuaternionFromEulerAngles(m_globalRotation);
		}
	}

	m_modifiedByPhysics = true;
}

Physics::Transform Physics::Transform::UpdatePhysics()
{
	Transform transform(nullptr);
	transform.m_globalPosition = m_globalPosition;
	if (m_pCurrentObject->GetComponent<Collider>()->GetShapeType() == Physics::ShapeType::Capsule)
	{
		transform.m_globalRot = Quaternion::GetQuaternionFromEulerAngles(Vector3D(m_globalRotation.x, m_globalRotation.y, m_globalRotation.z + 90.f));
	}
	else
	{
		transform.m_globalRot = m_globalRot;
	}

	return transform;
}

void Physics::Transform::SetGlobalPosition(const Vector3D& pos)
{
	m_globalModifications.push_back(std::pair(GlobalModification::SET_POSITION,Vector4D(pos)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::SetGlobalRotation(const Vector3D& rot)
{

	m_globalModifications.push_back(std::pair(GlobalModification::SET_ROTATION_EULER,Vector4D(rot)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::SetGlobalRotation(const Quaternion& rot)
{
	m_globalModifications.push_back(std::pair(GlobalModification::SET_ROTATION_QUATERNION,Vector4D(rot.x,rot.y,rot.z,rot.w)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::SetGlobalScale(const Vector3D& sca)
{
	m_globalModifications.push_back(std::pair(GlobalModification::SET_SCALE,Vector4D(sca)));

	if (GetGameObject()->HasComponent<Physics::Collider>())
	{
		Physics::Collider* coll = GetGameObject()->GetComponent<Physics::Collider>();
		coll->SetScale(coll->GetScale()); //Update collider with transform new scale
	}

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::SetLocalPosition(const Vector3D& pos)
{
	m_localModifications.push_back(std::pair(LocalModification::SET_POSITION,Vector4D(pos)));

	if (!m_localsHasBeenModified)
		m_localsHasBeenModified = true;
}

void Physics::Transform::SetLocalRotation(const Vector3D& rot)
{
	m_localModifications.push_back(std::pair(LocalModification::SET_ROTATION_EULER,Vector4D(rot)));

	if (!m_localsHasBeenModified)
		m_localsHasBeenModified = true;
}

void Physics::Transform::SetLocalRotation(const Quaternion& rot)
{
	m_localModifications.push_back(std::pair(LocalModification::SET_ROTATION_QUATERNION,Vector4D(rot.x,rot.y,rot.z,rot.w)));

	if (!m_localsHasBeenModified)
		m_localsHasBeenModified = true;
}

void Physics::Transform::SetLocalScale(const Vector3D& sca)
{
	m_localModifications.push_back(std::pair(LocalModification::SET_SCALE,Vector4D(sca)));

	if (GetGameObject()->HasComponent<Physics::Collider>())
	{
		Physics::Collider* coll = GetGameObject()->GetComponent<Physics::Collider>();
		coll->SetScale(coll->GetScale()); //Update collider with transform new scale
	}

	if (!m_localsHasBeenModified)
		m_localsHasBeenModified = true;
}

void Physics::Transform::CalculateModelMatrix()
{
	m_localModelMatrix = Matrix4D::CreateTransformMatrix(m_localPosition,m_localRot,m_localScale);
}

void Physics::Transform::CalculateMVP(Matrix4D vp)
{
	m_mvp = vp * m_globalModelMatrix;
}

void Physics::Transform::UpdateAllGlobalVariables()
{
	m_localModelMatrix = Matrix4D::CreateTransformMatrix(m_localPosition,m_localRot,m_localScale);

	if (HasParent())
	{
		m_globalPosition = m_pParent->GetGlobalPosition();
		m_globalPosition += m_pParent->GetVectorRight() * m_localPosition.x;
		m_globalPosition += m_pParent->GetVectorUp() * m_localPosition.y;
		m_globalPosition += m_pParent->GetVectorForward() * m_localPosition.z;
		m_globalRot = m_pParent->GetGlobalRotationInQuaternion() * m_localRot;
		m_globalRotation = Quaternion::GetEulerAnglesFromQuaternion(m_globalRot);
		m_globalScale = m_pParent->GetGlobalScale();
		m_globalScale.x *= m_localScale.x;
		m_globalScale.y *= m_localScale.y;
		m_globalScale.z *= m_localScale.z;
		m_globalModelMatrix = Matrix4D::CreateTransformMatrix(m_globalPosition,m_globalRot,m_globalScale);
	}
	else
	{
		m_globalModelMatrix = m_localModelMatrix;
		m_globalPosition = m_localPosition;
		m_globalRotation = m_localRotation;
		m_globalRot = Quaternion::GetQuaternionFromEulerAngles(m_globalRotation);
		m_globalScale = m_localScale;
	}
}

void Physics::Transform::UpdateAllLocalVariables()
{
	m_globalModelMatrix = Matrix4D::CreateTransformMatrix(m_globalPosition,m_globalRot,m_globalScale);

	if (HasParent())
	{
		m_localPosition = m_globalPosition - m_pParent->GetGlobalPosition();
		m_localRotation = m_globalRotation - m_pParent->GetGlobalRotation();
		m_localRot = Quaternion::GetQuaternionFromEulerAngles(m_localRotation);
		m_localScale = m_globalScale;
		m_localScale.x /= m_pParent->GetGlobalScale().x;
		m_localScale.y /= m_pParent->GetGlobalScale().y;
		m_localScale.z /= m_pParent->GetGlobalScale().z;
		m_localModelMatrix = Matrix4D::CreateTransformMatrix(m_localPosition,m_localRot,m_localScale);

	}
	else
	{
		m_localModelMatrix = m_globalModelMatrix;
		m_localPosition = m_globalPosition;
		m_localRotation = m_globalRotation;
		m_localRot = Quaternion::GetQuaternionFromEulerAngles(m_localRotation);;
		m_localScale = m_globalScale;
	}
}

void Physics::Transform::ApplyLocalModifications()
{
	for (std::vector<std::pair<LocalModification,Vector4D>>::iterator index = m_localModifications.begin(); index != m_localModifications.end(); index++)
	{
		switch (index->first)
		{
		case LocalModification::SET_POSITION:
			m_localPosition = index->second;
			break;
		case LocalModification::SET_ROTATION_EULER:
			m_localRotation = index->second;
			m_localRot = Quaternion::GetQuaternionFromEulerAngles(m_localRotation);
			break;
		case LocalModification::SET_ROTATION_QUATERNION:
			m_localRot = Quaternion(index->second.w,index->second.x,index->second.y,index->second.z);
			m_localRotation = Quaternion::GetEulerAnglesFromQuaternion(m_localRot);
			break;
		case LocalModification::SET_SCALE:
			m_localScale = index->second;
			break;
		default:
			break;
		}
	}
}

void Physics::Transform::ApplyGlobalModifications()
{
	for (std::vector<std::pair<GlobalModification,Vector4D>>::iterator index = m_globalModifications.begin(); index != m_globalModifications.end(); index++)
	{
		switch (index->first)
		{
		case GlobalModification::SET_POSITION:
			m_globalPosition = index->second;
			break;
		case GlobalModification::SET_ROTATION_EULER:
			m_globalRotation = index->second;
			m_globalRot = Quaternion::GetQuaternionFromEulerAngles(m_globalRotation);
			break;
		case GlobalModification::SET_ROTATION_QUATERNION:
			m_globalRot = Quaternion(index->second.w,index->second.x,index->second.y,index->second.z);
			m_globalRotation = Quaternion::GetEulerAnglesFromQuaternion(m_globalRot);
			break;
		case GlobalModification::SET_SCALE:
			m_globalScale = index->second;
			break;
		case GlobalModification::TRANSLATE:
			m_globalPosition += index->second;
			break;
		case GlobalModification::ROTATE_EULER:
			m_globalRotation += index->second;
			m_globalRot = Quaternion::GetQuaternionFromEulerAngles(m_globalRotation);
			break;
		case GlobalModification::ROTATE_QUATERNION:
			m_globalRot *= Quaternion(index->second.w,index->second.x,index->second.y,index->second.z);
			m_globalRotation = Quaternion::GetEulerAnglesFromQuaternion(m_globalRot);
			break;
		case GlobalModification::SCALE:
			m_globalScale.x *= index->second.x;
			m_globalScale.y *= index->second.y;
			m_globalScale.z *= index->second.z;
			break;
		default:
			break;
		}
	}
}

Vector3D Physics::Transform::GetVectorUp()
{
	return Vector3D::Normalize(m_globalRot * Vector3D::up);
}

Vector3D Physics::Transform::GetVectorRight()
{
	return Vector3D::Normalize(m_globalRot * Vector3D::right);
}

Vector3D Physics::Transform::GetVectorForward()
{
	return Vector3D::Normalize(m_globalRot * Vector3D::forward);
}

void Physics::Transform::Translate(const Vector3D& translation)
{
	m_globalModifications.push_back(std::pair(GlobalModification::TRANSLATE,Vector4D(translation)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::Rotate(const Vector3D& eulerRotation)
{
	m_globalModifications.push_back(std::pair(GlobalModification::ROTATE_EULER,Vector4D(eulerRotation)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::Rotate(const Quaternion& quaternionRotation)
{
	m_globalModifications.push_back(std::pair(GlobalModification::ROTATE_QUATERNION,Vector4D(quaternionRotation.x,quaternionRotation.y,quaternionRotation.z,quaternionRotation.w)));

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

void Physics::Transform::Scale(const Vector3D& scaling)
{
	m_globalModifications.push_back(std::pair(GlobalModification::SCALE,Vector4D(scaling)));

	if (GetGameObject()->HasComponent<Physics::Collider>())
	{
		Physics::Collider* coll = GetGameObject()->GetComponent<Physics::Collider>();
		coll->SetScale(coll->GetScale()); //Update collider with transform new scale
	}

	if (!m_globalsHasBeenModified)
		m_globalsHasBeenModified = true;
}

bool Physics::Transform::VerifyChild(Transform* child)
{
	for (std::vector<Transform*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		if ((*iter) == child)
		{
			//LWARNING("You try to add a child that already is a child of this object");
			return false;
		}
	}

	return true;
}

bool Physics::Transform::VerifyChild(Transform* parent, Transform* child)
{
	bool state = true;
	std::vector<Transform*> children = parent->GetChildrenArray();
	for(std::vector<Transform*>::iterator iter = children.begin(); iter != children.end(); iter++)
	{
		if ((*iter) == child )
		{
			state =  false;
		}
	}
	if (child->HasParent() && state)
		state = VerifyChild(parent, child->GetParent());

		return state;
}

void Physics::Transform::AddChild(Transform* transform)
{
	if (!VerifyChild(transform)) return;
	if (!VerifyChild(transform,this)) return;

	if (transform->GetParent() != this)
	{
		if (transform->HasParent()) transform->RemoveParent(true);
	}

	m_children.push_back(transform);

	if (transform->GetParent() == this) return;
	transform->AddParent(this);
}

void Physics::Transform::AddParent(Transform* transform)
{
	m_pParent = transform;
	m_pParent->AddChild(this);
	UpdateAllLocalVariables();
}

void Physics::Transform::RemoveParent(bool isStart)
{
	if (m_pParent == nullptr) return;

	if (isStart) m_pParent->RemoveChild(this);
	m_pParent = nullptr;

	UpdateAllLocalVariables();
	SetLocalPosition(m_localPosition);
	SetLocalRotation(m_localRotation);
	SetLocalScale(m_localScale);
}

void Physics::Transform::RemoveChild(Transform* transform)
{
	if (VerifyChild(transform))
	{
		LWARNING("You try to remove a child that doesn't exist");
		return;
	}

	for (std::vector<Transform*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		if ((*iter) == transform )
		{
			(*iter)->RemoveParent(false);
			m_children.erase(iter);
			m_children.shrink_to_fit();
			break;
		}
	}
}

void Physics::Transform::RemoveChilds()
{
	for (std::vector<Transform*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
		(*iter)->RemoveParent(false);

	m_children.clear();
}

void Physics::Transform::Update(Physics::Transform& transform)
{

}

void Physics::Transform::ApplyEditorChange()
{
	SetLocalPosition(m_localPosition);
	SetLocalRotation(m_localRotation);
	SetLocalScale(m_localScale);
}