#pragma once
#include <Scene/Component.h>
#include <Sound/AudioManager.h>
#include <Resources/WrapperAudio.h>
#include <Generated/SoundListner.rfkh.h>
 
namespace SoundSystem NAMESPACE()
{
	class CLASS()  LICH_ENGINE SoundListner : public SceneManagement::Component
	{
	public:
		SoundListner(SceneManagement::GameObject * go);
		~SoundListner();

		 void Update(Physics::Transform & transform) override;
		 void LoadData(std::string data) override ;
		 std::string SaveData() override ;
		 void ApplyEditorChange() override ;

	private:

		SoundSystem_SoundListner_GENERATED
	};

};
File_SoundListner_GENERATED
