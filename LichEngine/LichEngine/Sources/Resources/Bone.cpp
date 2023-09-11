#include <pch.h>
#include <Animation/Bone.h>
#include <Maths.h>


namespace AnimationSystem
{
	Bone::Bone()
	{
		m_pParent = nullptr;
	}


	Bone::Bone(const int boneIndex, const Matrix4D mat, const char* name, Bone* parent) :
		m_index(boneIndex), m_offset(mat), m_name(name), m_pParent(parent)
	{

	}

	Bone::~Bone()
	{

	}

	void Bone::AddChild(Bone* bone)
	{
		m_pChildren.push_back(bone);
	}

	void Bone::InitTPoseMatrix()
	{
		if (m_pParent != nullptr)
			m_globalModelMatrix = m_pParent->GetGlobalModelMatrix() * m_localModelMatrix;
		else
			m_globalModelMatrix = m_localModelMatrix;

		m_globalPosition = Matrix4D::GetPositionFromMatrix(m_globalModelMatrix);
		m_globalRotation = Quaternion::GetQuaternionFromEulerAngles(Matrix4D::GetRotationFromMatrix(m_globalModelMatrix));

		for (size_t i = 0; i < m_pChildren.size(); i++)
			m_pChildren[i]->InitTPoseMatrix();
	}

	void Bone::SetLocalTransform(const Matrix4D& localTransform)
	{ 
		m_localModelMatrix = localTransform;
		m_localPosition = Matrix4D::GetPositionFromMatrix(m_localModelMatrix);
		m_localRotation = Quaternion::GetQuaternionFromEulerAngles(Matrix4D::GetRotationFromMatrix(m_localModelMatrix));
	}

	float Bone::GetScaleFactor(const float& lastTimeStamp, const float& nextTimeStamp, const float& animationTime)
	{
		float scaleFactor = 0.f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}

	void Bone::Update(const Animation* currentAnim, const BoneAnimInfo* info, const float currentTime)
	{
		Vector3D finalPosition;
		Quaternion finalRotation;

		GetPosAndRot(currentAnim, info, finalPosition, finalRotation, currentTime);

		m_animationMatrix = Matrix4D::CreateTransformMatrix(finalPosition, finalRotation);
	}

	void Bone::GetPosAndRot(const Animation* animation, const BoneAnimInfo* info, Vector3D& pos, Quaternion& rot, float currentTime)
	{
		if (info->keyPositionCount == 1)
		{
			pos = info->keyPositions[0].position;
		}
		else
		{
			const int curPosIndex = animation->GetPositionIndex(currentTime, info);
			const int nextPosIndex = curPosIndex + 1;
			const float scaleFactorPos = GetScaleFactor(info->keyPositions[curPosIndex].timeStamp, info->keyPositions[nextPosIndex].timeStamp, currentTime);
			pos = Lerp<Vector3D>(info->keyPositions[curPosIndex].position, info->keyPositions[nextPosIndex].position, scaleFactorPos);
		}

		if (info->keyRotationCount == 1)
		{
			rot = info->keyRotations[0].rotation;
		}
		else
		{
			const int curRotIndex = animation->GetRotationIndex(currentTime, info);
			const int nextRotIndex = curRotIndex + 1;
			const float scaleFactorRot = GetScaleFactor(info->keyRotations[curRotIndex].timeStamp, info->keyRotations[nextRotIndex].timeStamp, currentTime);
			rot = Slerp(info->keyRotations[curRotIndex].rotation, info->keyRotations[nextRotIndex].rotation, scaleFactorRot);
		}
	}


	void Bone::UpdateWithTransition(const Animation* currentAnim, const Animation* nextAnimation, const BoneAnimInfo* info, const BoneAnimInfo* nextInfo, const float currentTime, const float nextCurrentTime,const float transitionCountDown, const float transitionTime)
	{
		Vector3D pos1, pos2, finalPos;
		Quaternion rot1, rot2, finalRot;
		const float t = transitionCountDown / transitionTime;

		GetPosAndRot(currentAnim, info, pos1, rot1, currentTime);
		GetPosAndRot(nextAnimation, nextInfo, pos2, rot2, nextCurrentTime);
		finalPos = Lerp<Vector3D>(pos1, pos2, t);
		finalRot = Slerp(rot1, rot2, t);
		m_animationMatrix = Matrix4D::CreateTransformMatrix(finalPos, finalRot);
	}
}