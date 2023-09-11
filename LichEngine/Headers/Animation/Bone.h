#pragma once
#include <Physics/Transform.h>
#include <Animation/Animation.h>


namespace AnimationSystem
{
	class LICH_ENGINE Bone
	{
	public:
		Bone();
		Bone(const int boneIndex, const Matrix4D mat, const char* name, Bone* parent = nullptr);
		~Bone();

		void InitTPoseMatrix();
		void AddChild(Bone* bone);
		void SetLocalTransform(const Matrix4D& localTransform);
		void Update(const Animation* currentAnim, const BoneAnimInfo* info, const float currentTime);
		void UpdateWithTransition(const Animation* currentAnim, const Animation* nextAnimation, const BoneAnimInfo* info, const BoneAnimInfo* nextInfo, const float currentTime, const float nextCurrentTime, const float transitionCountDown, const float transitionTime);

		inline int GetBoneIndex()						{ return m_index; }
		inline std::string GetBoneName()				{ return m_name; }
		inline Bone* GetBoneParent()					{ return m_pParent; }
		inline std::vector<Bone*> GetChildren()         { return m_pChildren; }
		inline Matrix4D GetLocalModelMatrix()			{ return m_localModelMatrix; }
		inline Matrix4D GetGlobalModelMatrix()			{ return m_globalModelMatrix; }
		inline Matrix4D GetOffset()					    { return m_offset; }
		inline Matrix4D GetAnimationMatrix()			{ return m_animationMatrix; }

		inline void SetBoneParent(Bone* parent)					{ m_pParent = parent; };
		inline void SetLocalPosition(const Vector3D pos)		{ m_localPosition = pos; }
		inline void SetGlobalPosition(const Vector3D pos)	    { m_globalPosition = pos; }
		inline void SetLocalRotation(const Quaternion rot)	    { m_localRotation = rot; }
		inline void SetGlobalRotation(const Quaternion rot)	    { m_globalRotation = rot; }
		
	private:
		int m_index = -1;

		std::string	m_name;

		Bone* m_pParent;
		std::vector<Bone*> m_pChildren;

		Vector3D m_localPosition;
		Vector3D m_globalPosition;
		Quaternion m_localRotation;
		Quaternion m_globalRotation;

		Matrix4D m_localModelMatrix;
		Matrix4D m_globalModelMatrix;
		Matrix4D m_animationMatrix;
		Matrix4D m_offset;

		void GetPosAndRot(const Animation* animation, const BoneAnimInfo* info, Vector3D& pos, Quaternion& rot, float currentTime);
		float GetScaleFactor(const float& lastTimeStamp, const float& nextTimeStamp, const float& animationTime);
	};
}