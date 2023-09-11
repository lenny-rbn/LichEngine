#pragma once
#include <Scene/Component.h>
#include <Resources/WrapperAudio.h>
#include <Resources/SoundInstance.h>
#include <Sound/AudioManager.h>
#include <ToolBox.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/SoundEvent.rfkh.h>

namespace SoundSystem NAMESPACE()
{
	class CLASS() LICH_ENGINE SoundEvent : public SceneManagement::Component
	{
	public:
		SoundEvent(SceneManagement::GameObject* go);
		~SoundEvent();

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		std::string SaveData() override;
		void ApplyEditorChange() override;

		void SetupSoundMinimunDistance(float distance);
		void SetupSoundMaximumDistance(float distance);
		void SetSound(const char* name, int index);
		void AddSound(const char* name);
		void PlaySound(int index);
		void StopSound(int index);
		unsigned int  GetSoundCount() { return m_soundArray.size(); }
		int  GetCurrentIndex() { return m_currentIndex; }
		float  GetMinDistance() { return m_distanceMin; }
		float  GetMaxDistance() { return m_distanceMax; }
		LichSoundSource* GetSound(int index) { return m_soundArray[index]->pSoundSource; }
		LichSoundSource* GetSound() { return m_soundArray[m_currentIndex]->pSoundSource; }
		Resources::SoundInstance* GetResource(int index) { return m_soundArray[index]; }
		Vector3D GetPosition() { return m_soundPosition; }



		FIELD() bool isLoop = false;
		FIELD() bool isPlayAsStart = false;
		FIELD() bool is3D = false;


	private:

		bool IsValidIndex(int index);
		Vector3D m_soundPosition;
		int m_currentIndex = 0;
		FIELD(ResourceTypeProperty(Resources::ResourceType::SOUND))  VectorContainer<Resources::SoundInstance> m_soundArray;


		FIELD() float m_distanceMin = 0.1f;
		FIELD() float m_distanceMax = 30.0f;
		bool m_started = false;
		bool m_stoped = false;

		SoundSystem_SoundEvent_GENERATED
	};


};
File_SoundEvent_GENERATED


