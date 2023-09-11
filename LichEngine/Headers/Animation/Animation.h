#pragma once
#include <Resources/Model.h>
#include <Resources/Resource.h>
#include <Generated/Animation.rfkh.h>

namespace AnimationSystem NAMESPACE()
{
	struct LICH_ENGINE KeyPosition
	{
		Vector3D position;
		float timeStamp = 0.f;
	};

	struct LICH_ENGINE KeyRotation
	{
		Quaternion rotation;
		float timeStamp = 0.f;
	};

	struct LICH_ENGINE BoneAnimInfo
	{
		int index = 0;
		int keyPositionCount = 0;
		int keyRotationCount = 0;
		std::vector<KeyPosition> keyPositions;
		std::vector<KeyRotation> keyRotations;
	};

	class CLASS() LICH_ENGINE Animation : public Resources::Resource
	{
	public:
		Animation();
		Animation(const std::string animName, const std::string path);
		Animation(const std::string path, const std::string animName, const int idInScene);
		~Animation();

		void Load(const std::string & path) override;
		std::string Save() override;
		bool IsCreated() override;
		void Unload() override;

		int GetPositionIndex(const float& animationTime, const BoneAnimInfo * infos) const;
		int GetRotationIndex(const float& animationTime, const BoneAnimInfo * infos) const;

		inline int GetBoneCount() { return m_boneCount; }
		inline float GetTickPerSeconds() { return m_tickPerSeconds; }
		inline float GetDuration() { return m_duration; }
		inline std::unordered_map<std::string, Resources::BoneInfo*>* GetBoneIDMap() { return &m_boneInfoMap; }
		inline BoneAnimInfo* FindBoneAnimInfo(std::string name) { return m_boneAnimInfosMap[name]; }

	private:
		int m_boneCount = 0;
		int m_idInScene = 0;

		float m_duration = 0.f;
		float m_tickPerSeconds = 0.f;

		std::unordered_map<std::string, Resources::BoneInfo*> m_boneInfoMap;
		std::unordered_map<std::string, BoneAnimInfo*> m_boneAnimInfosMap;

		void LoadAnimBone(const Resources::NodeAnimWrap * channel);
		void FillBoneInfoMap(Resources::NodeWrap * node, const Resources::SceneWrap * scene);
		AnimationSystem_Animation_GENERATED
	};
};
File_Animation_GENERATED