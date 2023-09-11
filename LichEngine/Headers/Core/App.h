#pragma once
#include <Core/TimeManager.h>
#include <Core/InputManager.h>
#include <Scene/SceneManager.h>
#include <Sound/AudioManager.h>
#include <Generated/App.rfkh.h>


namespace Core NAMESPACE()
{
    class CLASS() LICH_ENGINE App
    {
    private:
        AppSettings currentSettings;   

        Window* m_pWindow;
        TimeManager* m_pTimeManager;
        InputManager* m_pInputManager;
        SoundSystem::AudioManager* m_pAudioManager;
        Resources::ResourceManager* m_pResourcesManager;

        void CreateResources();
        void LoadApplicationSettings(std::string path);
        void WriteApplicationSettings();
        

        FIELD() SceneManagement::SceneManager* m_pSceneManager;

    public:
        App();
        ~App();
       
        void Run();
        void Draw();
        void Close();
        void Update();
        void Initialize();

        void QuitApplication() { isQuiting = true;}
        void SetWindow(LichWindow* window) { m_pWindow->SetWindow(window); };
        bool isQuiting = false;
        Resources::ResourceManager** GetResourceManager() { return &m_pResourcesManager; };
        InputManager** GetInputManager() { return &m_pInputManager; };
        TimeManager** GetTimeManager() { return &m_pTimeManager; };
        SceneManagement::SceneManager** GetSceneManager() { return &m_pSceneManager; };
        Window* GetWindow() { return m_pWindow; };
        

        Core_App_GENERATED 
    };
};
File_App_GENERATED