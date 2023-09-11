#pragma once
#include <Scene/Scene.h>
#include <Renderer/Kernel.h>
#include <Renderer/GBuffer.h>
#include <Resources/ResourceManager.h>
#include <Generated/RenderManager.rfkh.h>


namespace Renderer NAMESPACE()
{


	class CLASS() LICH_ENGINE RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void Init(const int width, const int height);
		void SetupShaders();
		void RecreateFrameBuffer(const int width, const int height);
		void Draw(SceneManagement::Scene* pScene);
		void DrawForPicking(SceneManagement::Scene* pScene);
		void Destroy();

		void SetCamera(CameraRender::Camera * cam) { m_pCamera = cam; }
		CameraRender::Camera* GetCamera() { return m_pCamera; }
		uint GetRenderTexture() { return m_colorTexture; }
		bool IsDrawing() { return m_isDrawing; }
		Vector2D GetWindowSize() { return Vector2D(m_width, m_height); }

		static Vector2D GetViewportSize() { return Vector2D(m_width, m_height); }

	private:
		float distanceCullingLights;

		GBuffer* m_pGBuffer;
		Kernels* m_pKernels;
		CameraRender::Camera* m_pCamera;

		uint m_hdrFBO;
		uint m_quadVAO;
		uint m_quadVBO;
		uint m_ssaoFBO;
		uint m_rboDepth;
		uint m_ssaoBlurFBO;
		uint m_pingpongFBO[2];
		uint m_colorBuffers[2];
		uint m_ssaoColorBuffer;
		uint m_pingpongBuffers[2];
		uint m_ssaoColorBufferBlur;
		uint m_colorBuffer = 0;
		uint m_colorTexture = 0;
		uint m_UIVAO = 0;
		uint m_UIVBO = 0;
		uint m_UIImageVAO = 0;
		uint m_UIImageVBO = 0;
		uint m_pickingBuffer = 0;
		uint m_pickingTexture = 0;

		static int m_width;
		static int m_height;
		bool m_isDrawing;

		Resources::Shader* m_pGeometryShader;
		Resources::Shader* m_pSsaoShader;
		Resources::Shader* m_pBlurShader;
		Resources::Shader* m_pHdrShader;
		Resources::Shader* m_pGaussianBlurShader;
		Resources::Shader* m_pPostprocessShader;
		Resources::Shader* m_pPBRShader;
		Resources::Shader* m_pPBRForwardShader;
		Resources::Shader* m_pSkyboxShader;
		Resources::Shader* m_pShadowDirShader;
		Resources::Shader* m_pShadowPointShader;
		Resources::Shader* m_pUITextShader;
		Resources::Shader* m_pUIImageShader;
		Resources::Shader* m_pGizmoShader;

		void ShadowPass(std::vector < SceneManagement::ObjInfo*> obj, LightSystem::LightManager * lightManager, Physics::Transform* cameraTransform);
		void GenerateShadowMap(Resources::Shader* shader, LightSystem::Light* light, std::vector<SceneManagement::ObjInfo*> objects, Physics::Transform* cameraTransform);
		void GeometryPass(std::vector < SceneManagement::ObjInfo*> objects, Physics::Transform* cameraTransform);
		void GenerateSSAOTexture(CameraRender::Camera * camera);
		void BlurSSAOTexture();
		void DrawSkybox(CameraRender::Camera* camera);
		void LightningPassPBR(LightSystem::LightManager * lightManager, Physics::Transform * cameraTransform);
		void LightningPassPBRForward(std::vector < SceneManagement::ObjInfo*> objects, LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform);
		void ColliderPass(Physics::PhysicsManager* manager, bool drawAllColliders, Physics::Transform* cameraTransform);
		void GizmoPass(SceneManagement::GameObject* selectedObject, SceneManagement::GizmosModificationMode modificationMode, Physics::Transform* cameraTransform);
		void PostProcessingPass();
		void UIPass(std::map<std::string, SceneManagement::GameObject*> gameObjects);

		void SendShadowMapToShader(LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform, int indexStart);

		void RenderToScreenQuad();

		Renderer_RenderManager_GENERATED
	};
}
File_RenderManager_GENERATED