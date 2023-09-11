#include <pch.h>
#include <Animation/Bone.h>
#include <Animation/Animator.h>
#include <Animation/Animation.h>
#include <Physics/Transform.h>
#include <Animation/Skeleton.h>
#include <ToolBox.h>
#include <Debugs/Log.h>
#include <sstream>
#include <Generated/Skeleton.rfks.h>

namespace AnimationSystem
{
	Skeleton::Skeleton(const std::string name, const std::string path)
	{
		std::filesystem::path filepath(path);
		this->path = path;
		this->name = Utils::remove_extension(filepath.filename().string());
		m_pBones.resize(200);
	}

	Skeleton::~Skeleton()
	{

	}

	void Skeleton::Load(const std::string& path)
	{
		std::string data, realPath;
		Utils::ReadFile(path, data);
		std::istringstream dataStringStream(data);
		std::getline(dataStringStream, realPath);

		Resources::ImporterWrap importer;
		const Resources::SceneWrap* scene = Resources::LoadScene(importer, realPath, 0);

		m_boneSkeletonCount = 0;
		FillBoneInfoMap(scene->mRootNode, scene);
		m_pBones.resize(m_boneSkeletonCount);

		ProcessBone(nullptr, scene->mRootNode, scene);
		m_pRootBone->InitTPoseMatrix();

		loaded = true;
		LINFO(path);
	}

	void Skeleton::FillBoneInfoMap(Resources::NodeWrap* node, const Resources::SceneWrap* scene)
	{
		for (int i = 0; i < (int)node->mNumMeshes; i++)
		{
			Resources::MeshWrap* mesh = scene->mMeshes[node->mMeshes[i]];
			for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
			{
				std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
				if (m_boneInfoMap.find(boneName) == m_boneInfoMap.end())
				{
					Resources::BoneInfo newBoneInfo;
					newBoneInfo.id = m_boneSkeletonCount;
					newBoneInfo.offset = Resources::ConvertMatrix4x4WrapToMatrix4D(mesh->mBones[boneIndex]->mOffsetMatrix);
					m_boneInfoMap[boneName] = newBoneInfo;
					m_boneSkeletonCount++;
				}
			}
		}

		for (int i = 0; i < (int)node->mNumChildren; i++)
			FillBoneInfoMap(node->mChildren[i], scene);
	}

	void Skeleton::ProcessBone(Bone* parentBone, Resources::NodeWrap* node, const Resources::SceneWrap* scene)
	{
		int boneID = -1;
		Matrix4D offset = Matrix4D(0.f);

		if (m_boneInfoMap.find(node->mName.data) == m_boneInfoMap.end())
		{
			boneID = m_boneSkeletonCount;
			m_boneSkeletonCount++;
			m_pBones.resize(m_boneSkeletonCount + 1);
		}
		else
		{
			boneID = m_boneInfoMap[node->mName.data].id;
			offset = m_boneInfoMap[node->mName.data].offset;
		}

		Bone* pNewBone = new Bone(boneID, offset, node->mName.data, parentBone);
		pNewBone->SetLocalTransform(Resources::ConvertMatrix4x4WrapToMatrix4D(node->mTransformation));
		this->m_pBones[pNewBone->GetBoneIndex()] = pNewBone;

		if (parentBone != nullptr)
			parentBone->AddChild(pNewBone);
		else
			m_pRootBone = pNewBone;

		for (int i = 0; i < (int)node->mNumChildren; i++)
			ProcessBone(pNewBone, node->mChildren[i], scene);
	}

	std::string Skeleton::Save()
	{
		std::string content = "3,";
		content += path + ',';
		content += name + ',';
		return content;
	}

	bool Skeleton::IsCreated()
	{
		return loaded;
	}

	void Skeleton::Unload()
	{
		for (int i = 0; i < m_pBones.size(); i++)
			delete m_pBones[i];
	}
}
