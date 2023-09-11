#pragma once
#include <Editor/HierachyWindow.h>

namespace Core
{
	class App;
	class Window;
}

namespace SceneManagement
{
	class SceneManager;
	class GameObject;
}

namespace CameraRender
{
	class Camera;
}

namespace Renderer
{
	class RenderManager;
}

namespace EditorLich
{
	enum class AxeSelected
	{
		NONE = 0,
		X,
		Y,
		Z,
	};

	class GameViewport : public IEditorWindow
	{
	public:
		GameViewport(Renderer::RenderManager* renderManager,Core::App* app, SceneManagement::SceneManager* scenManagerPtr);
		~GameViewport();

		void Update() override;
		void SetHierarchyWindow(HierachyWindow* window) { m_pHierachyWindow = window; };

	private:
		const char* m_windowName = "Game Viewport";

		bool ResizeViewport();
		void DrawSceneViewport();
		void CalculateResize();
		void CameraInputManagement();
		void HandleGizmosInputs(Vector2D mousePosition);

		Core::App* m_pApp = nullptr;
		Core::Window* m_pWindow = nullptr;
		Renderer::RenderManager* m_pRenderManager;
		HierachyWindow* m_pHierachyWindow;

		SceneManagement::SceneManager* m_pSceneManager;
		SceneManagement::GameObject* m_pCamEditor;
		CameraRender::Camera* m_pCam;

		Vector2D m_size;
		Vector2D m_prevSize;
		Vector2D m_windowSize;
		Vector2D m_prevMousePos;
		bool m_isResize = false;
		bool m_isOpen = true;
		AxeSelected axe = AxeSelected::NONE;
	};
}