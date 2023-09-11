#include <pch.h>
#include <Core/TimeManager.h>
#include <Resources/ResourceManager.h>
#include <Animation/Bone.h>
#include <Debugs/Log.h>

#include <Generated/Animator.rfks.h>

namespace AnimationSystem
{
	Animator::Animator(SceneManagement::GameObject* gO)
	{
		p_pGameObject = gO;
		componentType = SceneManagement::ComponentType::ANIMATOR;
		m_currentAnimationIndex = 0;
		m_currentTime = 0.f;
		m_nextCurrentTime = 0.f;
		m_pSkeleton = nullptr;
		m_matrices.resize(100);

		for (int i = 0; i < 100; i++)
			m_matrices.push_back(Matrix4D(0.f));
	}

	Animator::~Animator()
	{

	}

	void Animator::Update(Physics::Transform& transform)
	{
		if (m_pAnimations.size() == 0)
		{
			m_currentAnimationIndex = 0;
			m_pCurrentAnim = nullptr;
			m_pAnimations.push_back(nullptr);
		}
			
		if (m_pSkeleton != nullptr && IsReady() && m_pCurrentAnim != nullptr)
		{
			if (m_activeAnimation && transform.IsPlaying())
			{
				if (m_onTransition)
					TransitionBetweenAnimation();
				else
					RunAnimation();
			}

			Matrix4D identity = Matrix4D(0.f);
			identity.Identity();

			CalculateBoneTransform(m_pSkeleton->GetRoot(), identity);
		}
	}

	void Animator::CalculateBoneTransform(Bone* bone, Matrix4D transformGlobalParent)
	{
		const std::string& boneName = bone->GetBoneName();
		Matrix4D boneLocalTransform = bone->GetLocalModelMatrix();

		const BoneAnimInfo* infoBoneAnim = m_pCurrentAnim->FindBoneAnimInfo(boneName);

		if (!m_onTransition)
		{
			if (infoBoneAnim)
			{
				bone->Update(m_pCurrentAnim, infoBoneAnim, m_currentTime);
				boneLocalTransform = bone->GetAnimationMatrix();
			}
		}
		else
		{
			const BoneAnimInfo* nextInfoBoneAnim = m_pNextAnim->FindBoneAnimInfo(boneName);
			if (infoBoneAnim && nextInfoBoneAnim)
			{
				bone->UpdateWithTransition(m_pCurrentAnim, m_pNextAnim, infoBoneAnim, nextInfoBoneAnim, m_currentTime, m_nextCurrentTime, m_transitionCountdown, m_transitionTime);
				boneLocalTransform = bone->GetAnimationMatrix();
			}
		}

		Matrix4D globalTransformation = transformGlobalParent * boneLocalTransform;

		Resources::BoneInfo* info = (*m_pBoneInfoMap)[boneName];
		if (info != nullptr)
			m_matrices[info->id] = globalTransformation * info->offset;

		for (int i = 0; i < bone->GetChildren().size(); i++)
			CalculateBoneTransform(bone->GetChildren()[i], globalTransformation);
	}

	void Animator::LoadData(std::string data)
	{
		size_t index = 0;
		size_t baseIndex = 0;
		index = data.find(',', baseIndex);
		m_pSkeleton = Resources::ResourceManager::GetResource<Skeleton>(data.substr(baseIndex, index - baseIndex));
		baseIndex = index + 1;
		index = data.find(',', baseIndex + 1);
		int animNumber = std::stoi(data.substr(baseIndex, index - baseIndex));
		baseIndex = index + 1;
		for (size_t i = 0; i < animNumber; i++)
		{
			index = data.find(',', baseIndex + 1);
			AddAnimation(data.substr(baseIndex, index - baseIndex).c_str());
			baseIndex = index + 1;
		}
		m_pCurrentAnim = m_pAnimations[0];
	}

	std::string Animator::SaveData()
	{
		std::string content = "~7,";
		content += m_pSkeleton->name + ',';
		content += std::to_string(m_pAnimations.size()) + ',';
		for (int i = 0; i < m_pAnimations.size(); i++)
			content += m_pAnimations[i]->name + ',';

		return content;
	}

	void Animator::ApplyEditorChange()
	{
		if (m_currentAnimationIndex > m_pAnimations.size())
		{
			m_pCurrentAnim = m_pAnimations[0];
			m_currentAnimationIndex = 0;
		}

		m_pCurrentAnim = m_pAnimations[m_currentAnimationIndex];
		m_pBoneInfoMap = m_pCurrentAnim->GetBoneIDMap();
	}

	void Animator::StartTransition(const int nextAnimationIndex, const float transitionTime)
	{
		bool nextAnimSet = SetNextAnimation(nextAnimationIndex);
		if (!nextAnimSet)
			return;

		m_transitionTime = transitionTime;
		m_transitionCountdown = 0.f;
		m_onTransition = true;
		m_currentTime = 0.f;
	}

	std::vector<Resources::Resource*> Animator::GetResourcesUsed()
	{
		std::vector<Resources::Resource*> resources;

		resources.push_back(m_pSkeleton);


		for (std::vector<Animation*>::iterator index = m_pAnimations.begin(); index != m_pAnimations.end(); index++)
			resources.push_back(*index);

		return resources;
	}

	void Animator::AddAnimation(const char* name)
	{
		Animation* anim = Resources::ResourceManager::GetResource<Animation>(name);
		if (anim != nullptr)
			m_pAnimations.push_back(anim);

		if (anim != nullptr && m_pCurrentAnim == nullptr)
		{
			m_pCurrentAnim = m_pAnimations[0];
			m_currentAnimationIndex = 0;
			m_pBoneInfoMap = m_pCurrentAnim->GetBoneIDMap();
		}
	}

	void Animator::RunAnimation()
	{
		if (m_pCurrentAnim)
		{
			m_currentTime += m_speed * m_pCurrentAnim->GetTickPerSeconds() * (float)Core::TimeManager::GetDeltaTime();
			m_currentTime = fmod(m_currentTime, m_pCurrentAnim->GetDuration());
		}
	}

	void Animator::TransitionBetweenAnimation()
	{
		m_ratioTransition = m_transitionCountdown / m_transitionTime;

		double multiplier = (double)m_pNextAnim->GetDuration() / (double)m_pCurrentAnim->GetDuration();
		const double timeScale = Lerp<double>(multiplier, 1.0, m_ratioTransition);
		const double timeScale2 = Lerp<double>(1.0, 1.0 / multiplier, m_ratioTransition);

		m_currentTime += m_speed *(float)timeScale2 * m_pCurrentAnim->GetTickPerSeconds() * (float)Core::TimeManager::GetDeltaTime();
		m_currentTime = fmod(m_currentTime, m_pCurrentAnim->GetDuration());

		m_nextCurrentTime += m_speed * (float)timeScale * m_pNextAnim->GetTickPerSeconds() * (float)Core::TimeManager::GetDeltaTime();
		m_nextCurrentTime = fmod(m_nextCurrentTime, m_pNextAnim->GetDuration());

		if(!m_pauseTransition)
			m_transitionCountdown += (float)Core::TimeManager::GetDeltaTime();

		if (m_transitionCountdown < m_transitionTime) return;

		FinishTransition();
	}

	void Animator::SetRatioTransition(const float ratio)
	{
		m_ratioTransition = ratio;
		m_transitionCountdown = m_ratioTransition * m_transitionTime;
	}

	void Animator::SetCurrentAnimation(const int index)
	{
		if (index < m_pAnimations.size() && index >= 0)
		{
			m_pCurrentAnim = m_pAnimations[index];
			m_currentAnimationIndex = index;
			m_pBoneInfoMap = m_pCurrentAnim->GetBoneIDMap();
		}
		else
		{
			LWARNING("This animation isn't contained in this animator \n");
		}
	}

	void Animator::SetCurrentAnimation(const std::string name)
	{
		Animation* animation = Resources::ResourceManager::GetResource<Animation>(name);

		if (animation == nullptr)
			return;

		for (int i = 0; i < m_pAnimations.size(); i++) //Check if the Animation already exists in the Animator
		{
			if (animation == m_pAnimations[i])
			{
				m_pCurrentAnim = m_pAnimations[i];
				m_currentAnimationIndex = i;
				m_pBoneInfoMap = m_pCurrentAnim->GetBoneIDMap();
				return;
			}
		}

		//If not add it in the animator
		m_pAnimations.push_back(animation);
		int index = (int)m_pAnimations.size() - 1;
		m_pCurrentAnim = m_pAnimations[index];
		m_currentAnimationIndex = index;
		m_pBoneInfoMap = m_pCurrentAnim->GetBoneIDMap();
	}

	bool Animator::SetNextAnimation(const int index)
	{
		if (index == m_currentAnimationIndex)
		{
			LWARNING("This is the already the current animation \n");
			return false;
		}

		if (index < m_pAnimations.size() && index >= 0)
		{
			m_pNextAnim = m_pAnimations[index];
			m_nextAnimationIndex = index;
			return true;
		}

		LWARNING("This animation isn't contained in this animator \n");
		return false;
	}

	void Animator::FinishTransition()
	{
		m_pCurrentAnim = m_pNextAnim;
		m_pNextAnim = nullptr;
		m_onTransition = false;
		m_currentAnimationIndex = m_nextAnimationIndex;
		m_transitionCountdown = 0;
	}

	void Animator::SetSkeleton(const std::string name)
	{
		Skeleton* skeleton = Resources::ResourceManager::GetResource<Skeleton>(name);
		if (skeleton != nullptr)
		{
			m_pSkeleton = skeleton;
		}
	}

	bool Animator::IsReady()
	{
		if (m_pSkeleton!=nullptr && !m_pSkeleton->loaded)
			return false;

		for (int i = 0; i < m_pAnimations.size(); i++)
		{
			if (m_pAnimations[i] == nullptr) continue;

			if (!m_pAnimations[i]->loaded)
				return false;


		}
		return true;
	}
}
