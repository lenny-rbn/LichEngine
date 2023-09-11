#pragma once
#include <Scene/Component.h>
#include <Animation/Skeleton.h>
#include <Animation/Animation.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Animator.rfkh.h>



namespace AnimationSystem NAMESPACE()
{

	class CLASS() LICH_ENGINE  Animator : public SceneManagement::Component
	{
	public:
		Animator(SceneManagement::GameObject * gO);
		~Animator();

		std::string SaveData() override;
		void ApplyEditorChange() override;
		void LoadData(std::string data) override;
		void Update(Physics::Transform & transform) override;

		bool IsReady();

		void AddAnimation(const char* name);
		void SetSkeleton(const std::string name);
		void SetCurrentAnimation(const int index);
		void SetRatioTransition(const float ratio);
		void SetCurrentAnimation(const std::string name);
		void StartTransition(const int nextAnimationIndex, const float transitionTime);
		std::vector<Resources::Resource*> GetResourcesUsed();

		inline void SetAnimationSpeed(const float speed) { m_speed = speed; }
		inline void SetPauseTransition(const bool pause) { m_pauseTransition = pause; }
		inline void PlayCurrentAnimation(const bool active) { m_activeAnimation = active; }

		inline std::vector<Matrix4D> GetMatrices() { return m_matrices; }
		inline Animation* GetCurrentAnimation() { return m_pAnimations[m_currentAnimationIndex]; }
		inline Animation* GetNextAnimation() { if (m_pNextAnim) return m_pNextAnim; else return nullptr; }

	private:
		void RunAnimation();
		void FinishTransition();
		void TransitionBetweenAnimation();
		bool SetNextAnimation(const int index);
		void CalculateBoneTransform(Bone * bone, Matrix4D transformGlobalParent);

		FIELD(ResourceTypeProperty(Resources::ResourceType::SKELETON))	Skeleton* m_pSkeleton;
		Animation* m_pNextAnim = nullptr;
		Animation* m_pCurrentAnim = nullptr;

		int m_nextAnimationIndex = 0;
		int m_currentAnimationIndex = 0;

		FIELD()	float m_speed = 1.f;
		float m_transitionTime = 0.f;
		float m_ratioTransition = 0.f;
		float m_transitionCountdown = 0.f;

		bool m_onTransition = false;
		bool m_activeAnimation = true;
		bool m_pauseTransition = false;

		std::vector<Matrix4D> m_matrices;
		float m_currentTime;
		float m_nextCurrentTime;
		FIELD(ResourceTypeProperty(Resources::ResourceType::ANIMATION))VectorContainer<Animation> m_pAnimations;
		std::unordered_map<std::string, Resources::BoneInfo*>* m_pBoneInfoMap;
		AnimationSystem_Animator_GENERATED
	};
};
File_Animator_GENERATED