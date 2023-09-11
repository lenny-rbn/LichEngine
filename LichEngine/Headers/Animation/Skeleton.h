#pragma once
#include <Resources/Model.h>
#include <Resources/Resource.h>
#include <Generated/Skeleton.rfkh.h>

namespace AnimationSystem NAMESPACE()
{
	class Bone;
	class Animator;

	class CLASS() LICH_ENGINE  Skeleton : public Resources::Resource
	{
	public:
		Skeleton(const std::string path, const std::string name);
		~Skeleton();

		void Load(const std::string & path) override;
		std::string Save() override;
		bool IsCreated() override;
		void Unload() override;

		inline int GetSkeletonBoneCount() { return m_boneSkeletonCount; }
		inline Bone* GetRoot() { return m_pRootBone; }
		inline Bone* GetBone(const int index) { return m_pBones[index]; }

	private:
		int m_boneSkeletonCount = 0;

		Bone* m_pRootBone;
		std::vector<Bone*> m_pBones;
		std::unordered_map<std::string, Resources::BoneInfo> m_boneInfoMap;

		void ProcessBone(Bone * parentBone, Resources::NodeWrap * node, const Resources::SceneWrap * scene);
		void FillBoneInfoMap(Resources::NodeWrap * node, const Resources::SceneWrap * scene);

		AnimationSystem_Skeleton_GENERATED
	};
};
File_Skeleton_GENERATED