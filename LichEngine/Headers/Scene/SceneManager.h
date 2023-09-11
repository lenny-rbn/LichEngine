#pragma once
#include <Core/InputManager.h>
#include <Renderer/RenderManager.h>
#include <Generated/SceneManager.rfkh.h>


namespace Core
{
	class App;
}

namespace SceneManagement NAMESPACE()
{
	enum SceneLoadType
	{
		SCENE_LOAD_SYNCHRO = 0,
		SCENE_LOAD_ASYNCHRO = 1,
	};

	enum SceneMode
	{
		SCENE_NORMAL = 0,
		SCENE_ADDITIVE = 1,
	};

	struct SceneInfo
	{
		const char* name;
		const char* path;
		bool reset;
		bool isCall;
	};

	class CLASS() LICH_ENGINE SceneManager
	{
	public:
		SceneManager();
		SceneManager(int screenWidth, int screenHeight, Core::InputManager* inputManager, Core::App* app);
		~SceneManager();

		Scene* GetCurrentScene();
		Scene** GetCurrentScenePtr();
		bool ChangeScene(std::string name, std::string path);
		bool ChangeScene(const char* name, const char* path, bool isPlay =false);
		void ChangeSceneInGame(const char* name, const char* path, bool reset);
		bool CreateNewScene(const char* name, const char* path);
		void ReloadCurrentScene();
		void CreateScene(std::string name, std::string path, bool isNew);
		void LoadSceneManagerData();
		void SaveSceneManagerData();
		void Update();
		void Draw();
		void Destroy();
		void SaveCurrentScene();
		void LoadCurrentScene();
		void SetCamera(CameraRender::Camera * cam) { m_pRenderManager->SetCamera(cam); }
		void SetNewCamera(CameraRender::Camera* cam) { m_pRenderManager->SetCamera(cam); m_pCurrentScene->SetCamera(cam); }
		void UseSceneCamera() { m_pCurrentScene->SetCamera(); m_pRenderManager->SetCamera(m_pCurrentScene->GetCamera());  }
		GameObject* GetGameObjectFromID(Vector4D id);
		Vector4D GetIDFromPicking(Vector2D viewportCoord, Vector2D viewportPos, Vector2D viewportSize);
		CameraRender::Camera* GetCamera() { return m_pCurrentScene->GetCamera(); }

		bool IsCurrentSceneIsPlaying() { return m_pCurrentScene->IsPlaying(); }
		void ChangeCurrentScenePlayingState();
		void ChangeSceneName(Scene* scene);
		CameraRender::SkyBox* GetSceneSkybox() { return m_pCurrentScene->GetSkybox(); }
		Core::InputManager* GetInputManager() { return m_pInputManager; }
		int GetFramerate();
		void QuitApp();

	private:
		FIELD() Renderer::RenderManager* m_pRenderManager; // Temp :  var is public to use in editor
		unsigned int m_currentSceneID = 0;
		Scene* m_pCurrentScene;
		Core::InputManager* m_pInputManager;

		Core::App* m_pApp;

		std::string m_lastSceneOpenPath;
		std::string m_lastSceneOpenName;
		
		SceneInfo sceneToLoadInfo;

		SceneManagement_SceneManager_GENERATED
	};
};
File_SceneManager_GENERATED