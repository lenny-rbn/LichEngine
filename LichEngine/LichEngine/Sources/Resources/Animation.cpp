#include <pch.h>
#include <Core/TimeManager.h>
#include <Resources/WrapperAssimp.h>
#include <Resources/ResourceManager.h>
#include <Debugs/Log.h>
#include <sstream>
#include <filesystem>
#include <Generated/Animation.rfks.h>


AnimationSystem::Animation::Animation()
{
	m_boneCount = 0;

	type = Resources::ResourceType::ANIMATION;
}

AnimationSystem::Animation::Animation(const std::string animName, const std::string path)
{
	m_boneCount = 0;
	m_idInScene = 0;
	std::filesystem::path filepath(path);
	this->path = path;
	this->name = Utils::remove_extension(filepath.filename().string());
	type = Resources::ResourceType::ANIMATION;
}

AnimationSystem::Animation::Animation(const std::string path, const std::string animName, const int idInScene)
{
	m_boneCount = 0;
	m_idInScene = idInScene;
	std::filesystem::path filepath(path);
	this->path = path;
	this->name = Utils::remove_extension(filepath.filename().string());
	type = Resources::ResourceType::ANIMATION;
}

AnimationSystem::Animation::~Animation()
{

}

void AnimationSystem::Animation::Load(const std::string& path)
{
	std::string data;
	Utils::ReadFile(path, data);
	std::string dataCut[2];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < 2; i++)
	{
		index = data.find(',', index + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	std::string realPath = dataCut[0];
	m_idInScene = std::stoi(dataCut[1]);

	Resources::ImporterWrap importer;
	const Resources::SceneWrap* scene = Resources::LoadScene(importer, realPath, 0);

	FillBoneInfoMap(scene->mRootNode, scene);

	if ((int)scene->mNumAnimations > m_idInScene)
	{
		Resources::AnimationWrap* anim = scene->mAnimations[m_idInScene];
		m_duration = (float)anim->mDuration;
		m_tickPerSeconds = (float)anim->mTicksPerSecond;
		Matrix4D identity = Matrix4D(0.f);
		identity.Identity();

		for (int i = 0; i < (int)anim->mNumChannels; i++)
		{
			if (m_boneInfoMap.find(anim->mChannels[i]->mNodeName.data) == m_boneInfoMap.end())
			{
				Resources::BoneInfo* info = new Resources::BoneInfo();
				info->id = m_boneCount;
				info->offset = identity;
				m_boneInfoMap[anim->mChannels[i]->mNodeName.data] = info;
				m_boneCount++;
			}

			Resources::NodeAnimWrap* channel = anim->mChannels[i];
			LoadAnimBone(channel);
		}
	}

	loaded = true;
	LINFO(realPath);
}

void AnimationSystem::Animation::LoadAnimBone(const Resources::NodeAnimWrap* channel)
{
	BoneAnimInfo* newBoneInfo = new BoneAnimInfo();
	newBoneInfo->index = (int)m_boneAnimInfosMap.size();
	newBoneInfo->keyPositionCount = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < newBoneInfo->keyPositionCount; ++positionIndex)
	{
		Vector3D position = Vector3D(channel->mPositionKeys[positionIndex].mValue.x, channel->mPositionKeys[positionIndex].mValue.y, channel->mPositionKeys[positionIndex].mValue.z);
		float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = position;
		data.timeStamp = timeStamp;
		newBoneInfo->keyPositions.push_back(data);
	}

	newBoneInfo->keyRotationCount = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < newBoneInfo->keyRotationCount; ++rotationIndex)
	{
		Quaternion rotation = Quaternion(channel->mRotationKeys[rotationIndex].mValue.w, channel->mRotationKeys[rotationIndex].mValue.x, channel->mRotationKeys[rotationIndex].mValue.y, channel->mRotationKeys[rotationIndex].mValue.z);
		float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.rotation = rotation;
		data.timeStamp = timeStamp;
		newBoneInfo->keyRotations.push_back(data);
	}

	m_boneAnimInfosMap[channel->mNodeName.data] = newBoneInfo;
}

void AnimationSystem::Animation::FillBoneInfoMap(Resources::NodeWrap* node, const Resources::SceneWrap* scene)
{
	for (int i = 0; i < (int)node->mNumMeshes; i++)
	{
		Resources::MeshWrap* mesh = scene->mMeshes[node->mMeshes[i]];
		for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
		{
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (m_boneInfoMap.find(boneName) == m_boneInfoMap.end())
			{
				Resources::BoneInfo* newBoneInfo = new Resources::BoneInfo();
				newBoneInfo->id = m_boneCount;
				newBoneInfo->offset = Resources::ConvertMatrix4x4WrapToMatrix4D(mesh->mBones[boneIndex]->mOffsetMatrix);
				m_boneInfoMap[boneName] = newBoneInfo;
				m_boneCount++;
			}
		}
	}

	for (int i = 0; i < (int)node->mNumChildren; i++)
		FillBoneInfoMap(node->mChildren[i], scene);
}

std::string AnimationSystem::Animation::Save()
{
	std::string content = "5,";
	content += path + ',';
	content += name + ',';
	return content;
}

bool AnimationSystem::Animation::IsCreated()
{
	return loaded;
}

void AnimationSystem::Animation::Unload()
{
	for (std::unordered_map<std::string, Resources::BoneInfo*>::iterator it = m_boneInfoMap.begin(); it != m_boneInfoMap.end(); it++)
		delete it->second;

	for (std::unordered_map<std::string, BoneAnimInfo*>::iterator it = m_boneAnimInfosMap.begin(); it != m_boneAnimInfosMap.end(); it++)
		delete it->second;
}

int AnimationSystem::Animation::GetPositionIndex(const float& animationTime, const BoneAnimInfo* infos) const
{
	for (int index = 0; index < infos->keyPositionCount - 1; ++index)
		if (animationTime < infos->keyPositions[(index + 1)].timeStamp)
			return index;

	return -1;
}

int AnimationSystem::Animation::GetRotationIndex(const float& animationTime, const BoneAnimInfo* infos) const
{
	for (int index = 0; index < infos->keyRotationCount - 1; ++index)
		if (animationTime < infos->keyRotations[(index + 1)].timeStamp)
			return index;

	return -1;
}

