#include <pch.h>
#include <Define.h>

#include <Renderer/Kernel.h>
#include <Resources/ResourceManager.h>
#include <Core/DirLight.h>
#include <Core/SpotLight.h>
#include <Core/PointLight.h>
#include <Physics/Transform.h>
#include <Physics/PhysicsManager.h>
#include <UI/Text.h>
#include <UI/Image.h>
#include <Scene/GameObject.h>
#include <Animation/Animator.h>
#include <Generated/RenderManager.rfks.h>

namespace Renderer
{
	int RenderManager::m_width = 0.f;
	int RenderManager::m_height = 0.f;

	RenderManager::RenderManager()
	{
		m_pGBuffer = nullptr;
	}

	RenderManager::~RenderManager()
	{

	}

	void RenderManager::Init(const int width, const int height)
	{
		m_width = width;
		m_height = height;
		distanceCullingLights = 15.f;

#ifdef LICH_EDITOR
		WrapperRHI::RHI::CreateRenderTexture(m_colorBuffer, m_colorTexture, m_width, height);
		WrapperRHI::RHI::CreateRenderTexture(m_pickingBuffer, m_pickingTexture, m_width, m_height);
#endif //  LICH_EDITOR

		m_pGBuffer = new GBuffer(m_width, height);
		m_pKernels = new Kernels();
		WrapperRHI::RHI::CreateOneChannelFrameBuffer(m_ssaoFBO, m_ssaoColorBuffer, m_width, height);
		WrapperRHI::RHI::CreateOneChannelFrameBuffer(m_ssaoBlurFBO, m_ssaoColorBufferBlur, m_width, height);
		WrapperRHI::RHI::CreateHDRFrameBuffer(m_hdrFBO, m_rboDepth, m_colorBuffers, m_width, height);
		WrapperRHI::RHI::CreatePingPongFrameBuffer(m_pingpongFBO, m_pingpongBuffers, m_width, height);
		WrapperRHI::RHI::CreateUIImageQuad(m_UIImageVAO, m_UIImageVBO);
		WrapperRHI::RHI::CreateUIQuad(m_UIVAO, m_UIVBO);
		WrapperRHI::RHI::CreateScreenQuad(m_quadVAO, m_quadVBO);

		SetupShaders();

		WrapperRHI::RHI::EnableOptions(GProperties::DEPTH_TEST);
		WrapperRHI::RHI::EnableOptions(GProperties::CULL_FACE);
		WrapperRHI::RHI::SetCullFace(GProperties::BACK);
		WrapperRHI::RHI::SetDepthFunc(GProperties::LESS);
	}

	void RenderManager::RecreateFrameBuffer(const int width, const int height)
	{
		Init(width, height);
		m_pCamera->SetScreenSize({ width,height });
	}

	void RenderManager::SetupShaders()
	{
		m_pGeometryShader = Resources::ResourceManager::GetResource<Resources::Shader>("geometryShader");
		m_pSsaoShader = Resources::ResourceManager::GetResource<Resources::Shader>("ssaoShader");
		m_pBlurShader = Resources::ResourceManager::GetResource<Resources::Shader>("blurShader");
		m_pHdrShader = Resources::ResourceManager::GetResource<Resources::Shader>("hdrShader");
		m_pGaussianBlurShader = Resources::ResourceManager::GetResource<Resources::Shader>("gaussianBlurShader");
		m_pPostprocessShader = Resources::ResourceManager::GetResource<Resources::Shader>("postprocessShader");
		m_pPBRShader = Resources::ResourceManager::GetResource<Resources::Shader>("PBRShader");
		m_pPBRForwardShader = Resources::ResourceManager::GetResource<Resources::Shader>("PBRForwardShader");
		m_pSkyboxShader = Resources::ResourceManager::GetResource<Resources::Shader>("skyboxShader");
		m_pShadowDirShader = Resources::ResourceManager::GetResource<Resources::Shader>("shadowDirLightShader");
		m_pShadowPointShader = Resources::ResourceManager::GetResource<Resources::Shader>("shadowPointLightShader");
		m_pUITextShader = Resources::ResourceManager::GetResource<Resources::Shader>("UITextShader");
		m_pUIImageShader = Resources::ResourceManager::GetResource<Resources::Shader>("UIImageShader");
		Resources::Shader* PickingShader = Resources::ResourceManager::GetResource<Resources::Shader>("PickingShader");
		m_pGizmoShader = Resources::ResourceManager::GetResource<Resources::Shader>("GizmoShader");

#pragma region SetupShader
		// Bind all Sampler2D to shaders
		// -----------------------------
		WrapperRHI::RHI::UseShader(m_pGeometryShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "text_diffuse", 0);
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "normal_map", 1);
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "height_map", 2);
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "metallic_map", 3);
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "roughness_map", 4);
		WrapperRHI::RHI::Send1iToShader(m_pGeometryShader, "ao_map", 5);

		WrapperRHI::RHI::UseShader(m_pSsaoShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pSsaoShader, "gPosition", 0);
		WrapperRHI::RHI::Send1iToShader(m_pSsaoShader, "gNormal", 1);
		WrapperRHI::RHI::Send1iToShader(m_pSsaoShader, "texNoise", 2);
		m_pKernels->SendKernelsToShader(m_pSsaoShader);

		WrapperRHI::RHI::UseShader(m_pBlurShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pBlurShader, "ssaoInput", 0);

		WrapperRHI::RHI::UseShader(m_pPBRShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "gPosition", 0);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "gNormal", 1);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "gAlbedo", 2);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "ssaoText", 3);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "metalRoughAOBloom", 4);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[0]", 5);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[1]", 6);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[2]", 7);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[3]", 8);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[4]", 9);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[5]", 10);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowCubeMaps[6]", 11);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowDirMaps[0]", 12);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowDirMaps[1]", 13);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowDirMaps[2]", 14);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowDirMaps[3]", 15);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowSpotMaps[0]", 16);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowSpotMaps[1]", 17);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowSpotMaps[2]", 18);
		WrapperRHI::RHI::Send1iToShader(m_pPBRShader, "shadowSpotMaps[3]", 19);

		WrapperRHI::RHI::UseShader(m_pPBRForwardShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "text_diffuse", 0);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "normal_map", 1);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "height_map", 2);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "metallic_map", 3);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "roughness_map", 4);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "ao_map", 5);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "ssaoText", 6);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[0]", 8);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[1]", 9);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[2]", 10);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[3]", 11);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[4]", 12);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[5]", 13);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowCubeMaps[6]", 14);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowDirMaps[0]", 15);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowDirMaps[1]", 16);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowDirMaps[2]", 17);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowDirMaps[3]", 18);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowSpotMaps[0]", 19);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowSpotMaps[1]", 20);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowSpotMaps[2]", 21);
		WrapperRHI::RHI::Send1iToShader(m_pPBRForwardShader, "shadowSpotMaps[3]", 22);

		WrapperRHI::RHI::UseShader(m_pHdrShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pHdrShader, "hdrBuffer", 0);

		WrapperRHI::RHI::UseShader(m_pGaussianBlurShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pGaussianBlurShader, "image", 0);

		WrapperRHI::RHI::UseShader(m_pPostprocessShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pPostprocessShader, "scene", 0);
		WrapperRHI::RHI::Send1iToShader(m_pPostprocessShader, "bloomBlur", 1);

		WrapperRHI::RHI::UseShader(m_pSkyboxShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pSkyboxShader, "skybox", 0);

		WrapperRHI::RHI::UseShader(m_pUITextShader->GetShaderProgram());
		WrapperRHI::RHI::Send1iToShader(m_pUITextShader, "text", 0);
#pragma endregion

	}

	void RenderManager::Draw(SceneManagement::Scene* pScene)
	{
		std::map<std::string, SceneManagement::GameObject*> gameObjects = pScene->GetGameObjects();
		LightSystem::LightManager* lightManager = pScene->GetLightManager();
		Physics::Transform* cameraTransform = pScene->GetCameraTransform();
		CameraRender::Camera* camera = cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>();
		std::vector<SceneManagement::ObjInfo*> cullMeshes = pScene->GetObjInfo();

		WrapperRHI::RHI::ClearColor(0, 0, 0, 0);
		if (pScene->GetCamera() == nullptr)
		{
			m_isDrawing = false;
			return;
		}
		ShadowPass(cullMeshes, lightManager, cameraTransform);
		GeometryPass(cullMeshes, cameraTransform);
		//GenerateSSAOTexture(camera);
		LightningPassPBR(lightManager, cameraTransform);
		DrawSkybox(camera);
		LightningPassPBRForward(cullMeshes, lightManager, cameraTransform); //Blending

#ifdef LICH_EDITOR
		if (!pScene->IsPlaying())
		{
			ColliderPass(pScene->GetPhysicManager(), pScene->GetDrawCollidersState(), cameraTransform);
			SceneManagement::Gizmos* gizmos = pScene->GetGizmos();
			GizmoPass(gizmos->GetGO(), gizmos->GetModificationMode(), cameraTransform);
		}
#endif // LICH_EDITOR

		PostProcessingPass();
		UIPass(gameObjects);
	}

	void RenderManager::DrawForPicking(SceneManagement::Scene* pScene)
	{
		std::map<Physics::Transform*, Mesh*> meshes = pScene->GetObjects();
		std::map<std::string, SceneManagement::GameObject*> gameObjects = pScene->GetGameObjects();
		
		Physics::Transform* cameraTransform = pScene->GetCameraTransform();
		if (pScene->GetCamera() == nullptr)
		{
			m_isDrawing = false;
			return;
		}
		WrapperRHI::RHI::SetActiveFramebuffer(m_pickingBuffer);
		WrapperRHI::RHI::ClearColorAndDepth();
		Resources::Shader* shader = Resources::ResourceManager::GetResource<Resources::Shader>("PickingShader");
		if (!shader->IsCreated())
			LWARNING("Shader not loaded or created");

		WrapperRHI::RHI::UseShader(shader->GetShaderProgram());
		WrapperRHI::RHI::Send3fToShader(shader, "viewPos", cameraTransform->GetGlobalPosition());
		WrapperRHI::RHI::SendMat4ToShader(shader, "viewMatrix", 1, false, &cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetViewMatrix().e[0]);

		for (std::map<Physics::Transform*, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			if (it->second->GetModel() != nullptr && it->second->GetModel()->IsCreated())
				WrapperRHI::RHI::DrawMeshForPicking(it->second, it->first, shader);
		}

		if (pScene->GetGizmos()->GetGO() != nullptr)
			WrapperRHI::RHI::DrawGizmosForPicking(pScene->GetGizmos()->GetGO()->GetTransform(), pScene->GetGizmos()->GetModificationMode(), cameraTransform, shader);
	}

	void RenderManager::ShadowPass(std::vector<SceneManagement::ObjInfo*> objects, LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform)
	{
		if (lightManager->GetLightNumber() > 0)
		{
			WrapperRHI::RHI::SetViewportSize(lightManager->GetLight(0)->GetSizeShadowMap());
		
			for (int i = 0; i < lightManager->GetLightNumber(); i++)
			{
				if (!lightManager->GetLight(i)->IsGeneratingShadow() || !lightManager->GetLight(i)->isEnable)
					continue;

				if ((lightManager->GetLight(i)->GetType() != LightSystem::TypeLight::DIR_LIGHT) && (lightManager->GetLight(i)->GetGameObject()->GetTransform()->GetGlobalPosition() - cameraTransform->GetGlobalPosition()).Magnitude() >= distanceCullingLights)
					continue;

				LightSystem::TypeLight type = lightManager->GetLight(i)->GetType();

				if (type == LightSystem::TypeLight::DIR_LIGHT || type == LightSystem::TypeLight::SPOT_LIGHT)
					GenerateShadowMap(m_pShadowDirShader, lightManager->GetLight(i), objects, cameraTransform);
				else if (type == LightSystem::TypeLight::POINT_LIGHT)
					GenerateShadowMap(m_pShadowPointShader, lightManager->GetLight(i), objects, cameraTransform);
			}

			WrapperRHI::RHI::SetViewportSize(m_width, m_height);
		}
	}

	void RenderManager::GeometryPass(std::vector < SceneManagement::ObjInfo*> objects, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_pGBuffer->GetGBuffer());
		WrapperRHI::RHI::ClearColorAndDepth();
		WrapperRHI::RHI::UseShader(m_pGeometryShader->GetShaderProgram());
		WrapperRHI::RHI::Send3fToShader(m_pGeometryShader, "viewPos", cameraTransform->GetGlobalPosition());
		WrapperRHI::RHI::Send1fToShader(m_pGeometryShader, "height_scale", 0.03f);
		WrapperRHI::RHI::SendMat4ToShader(m_pGeometryShader, "viewMatrix", 1, false, &cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetViewMatrix().e[0]);

		for (std::vector < SceneManagement::ObjInfo*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if ((*it)->mesh->IsForwardRendering() || (*it)->mesh->HasTransparentMaterials()) //Will be drawn in forward rendering
				continue;

			if(!(*it)->mesh->isEnable || (*it)->isDraw) continue;

			if ((*it)->animator)
				WrapperRHI::RHI::DrawMesh((*it), m_pGeometryShader);
			else
				WrapperRHI::RHI::DrawMeshNotAnimated((*it), m_pGeometryShader);
		}

		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::GenerateSSAOTexture(CameraRender::Camera* camera)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_ssaoFBO);
		WrapperRHI::RHI::ClearColor();
		WrapperRHI::RHI::UseShader(m_pSsaoShader->GetShaderProgram());

		WrapperRHI::RHI::ActivateAndBindTexture2D(0, m_pGBuffer->GetGViewPosition());
		WrapperRHI::RHI::ActivateAndBindTexture2D(1, m_pGBuffer->GetGViewNormal());
		WrapperRHI::RHI::ActivateAndBindTexture2D(2, m_pKernels->GetNoiseTexture());
		WrapperRHI::RHI::SendMat4ToShader(m_pSsaoShader, "projection", 1, false, &camera->GetProjection().e[0]);
		WrapperRHI::RHI::SendMat4ToShader(m_pSsaoShader, "viewMatrix", 1, false, &camera->GetViewMatrix().e[0]);
		RenderToScreenQuad();
		BlurSSAOTexture();
	}

	void RenderManager::BlurSSAOTexture()
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_ssaoBlurFBO);
		WrapperRHI::RHI::ClearColor();
		WrapperRHI::RHI::UseShader(m_pBlurShader->GetShaderProgram());
		WrapperRHI::RHI::ActivateAndBindTexture2D(0, m_ssaoColorBuffer);
		RenderToScreenQuad();
		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::LightningPassPBR(LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_hdrFBO);
		WrapperRHI::RHI::ClearColorAndDepth();
		WrapperRHI::RHI::UseShader(m_pPBRShader->GetShaderProgram());
		WrapperRHI::RHI::ActivateAndBindTexture2D(0, m_pGBuffer->GetGPosition());
		WrapperRHI::RHI::ActivateAndBindTexture2D(1, m_pGBuffer->GetGNormal());
		WrapperRHI::RHI::ActivateAndBindTexture2D(2, m_pGBuffer->GetGColor());
		WrapperRHI::RHI::ActivateAndBindTexture2D(3, m_ssaoColorBufferBlur);
		WrapperRHI::RHI::ActivateAndBindTexture2D(4, m_pGBuffer->GetGMetallicRoughness());
		SendShadowMapToShader(lightManager, cameraTransform, 5);
		lightManager->Update(m_pPBRShader, cameraTransform, distanceCullingLights);
		WrapperRHI::RHI::Send3fToShader(m_pPBRShader, "camPos", cameraTransform->GetGlobalPosition());
		WrapperRHI::RHI::Send1fToShader(m_pPBRShader, "far_plane", 30.f);

		RenderToScreenQuad();
		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::DrawSkybox(CameraRender::Camera* camera)
	{
		WrapperRHI::RHI::CopyDepth(m_pGBuffer->GetGBuffer(), m_hdrFBO, m_width, m_height);

		if (camera->GetSkyBox() != nullptr && camera->GetSkyBox()->IsCreated())
		{
			WrapperRHI::RHI::SetActiveFramebuffer(m_hdrFBO);
			WrapperRHI::RHI::UseShader(m_pSkyboxShader->GetShaderProgram());
			WrapperRHI::RHI::SendMat4ToShader(m_pSkyboxShader, "projection", 1, false, &camera->GetProjection().e[0]);
			WrapperRHI::RHI::SendMat4ToShader(m_pSkyboxShader, "viewMatrix", 1, false, &camera->GetViewMatrix().e[0]);
			WrapperRHI::RHI::DrawSkyBox(camera->GetSkyBox());
			WrapperRHI::RHI::SetActiveFramebuffer(0);
		}
	}

	void RenderManager::LightningPassPBRForward(std::vector < SceneManagement::ObjInfo*> objects, LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_hdrFBO);
		WrapperRHI::RHI::UseShader(m_pPBRForwardShader->GetShaderProgram());
		SendShadowMapToShader(lightManager, cameraTransform, 8);
		lightManager->Update(m_pPBRForwardShader, cameraTransform, distanceCullingLights);
		WrapperRHI::RHI::Send1fToShader(m_pPBRForwardShader, "far_plane", 30.f);
		WrapperRHI::RHI::Send1fToShader(m_pPBRForwardShader, "height_scale", 0.03f);
		WrapperRHI::RHI::Send3fToShader(m_pPBRForwardShader, "camPos", cameraTransform->GetGlobalPosition());
		WrapperRHI::RHI::SendMat4ToShader(m_pPBRForwardShader, "viewMatrix", 1, false, &cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetViewMatrix().e[0]);

		std::map<double, SceneManagement::ObjInfo*> sorted;
		for (std::vector < SceneManagement::ObjInfo*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (!(*it)->mesh->IsForwardRendering() || !(*it)->mesh->HasTransparentMaterials())
				continue;

			if ((*it)->mesh->GetModel()->IsCreated())
			{
				double distance = (cameraTransform->GetGlobalPosition() - (*it)->globalPosition).Magnitude();

				if (sorted[distance] != nullptr)
					distance += 0.0001;
				sorted[distance] = (*it);
			}
		}

		if (sorted.size() > 0)
		{
			WrapperRHI::RHI::EnableOptions(BLEND);
			WrapperRHI::RHI::SetBlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
			for (std::map<double, SceneManagement::ObjInfo*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
				WrapperRHI::RHI::DrawMesh(it->second, m_pPBRForwardShader);
			WrapperRHI::RHI::DisableOptions(BLEND);
		}

		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::ColliderPass(Physics::PhysicsManager* manager, bool drawAllColliders, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_hdrFBO);
		WrapperRHI::RHI::UseShader(m_pGizmoShader->GetShaderProgram());

		std::map<SceneManagement::GameObject*, Physics::RigidActor*> actors = manager->GetSimulationActors();
		for (std::map<SceneManagement::GameObject*, Physics::RigidActor*>::iterator index = actors.begin(); index != actors.end(); index++)
		{
			if (index->first->IsSelected() || drawAllColliders)
				WrapperRHI::RHI::DrawCollider(index->first, cameraTransform, m_pGizmoShader);
		}

		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::GizmoPass(SceneManagement::GameObject* selectedObject, SceneManagement::GizmosModificationMode modificationMode, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(m_hdrFBO);
		WrapperRHI::RHI::UseShader(m_pGizmoShader->GetShaderProgram());

		if (selectedObject != nullptr)
			WrapperRHI::RHI::DrawGizmos(selectedObject->GetTransform(), modificationMode, cameraTransform, m_pGizmoShader);

		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::PostProcessingPass()
	{
		//Bloom
		bool horizontal = true, first_iteration = true;
		WrapperRHI::RHI::UseShader(m_pGaussianBlurShader->GetShaderProgram());
		for (unsigned int i = 0; i < 10; ++i)
		{
			WrapperRHI::RHI::SetActiveFramebuffer(m_pingpongFBO[horizontal]);
			WrapperRHI::RHI::Send1iToShader(m_pGaussianBlurShader, "horizontal", horizontal);
			WrapperRHI::RHI::ActivateAndBindTexture2D(0, first_iteration ? m_colorBuffers[1] : m_pingpongBuffers[!horizontal]);
			RenderToScreenQuad();

			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		//HDR + Gamma Correction
		WrapperRHI::RHI::SetActiveFramebuffer(m_colorBuffer);
		WrapperRHI::RHI::ClearColorAndDepth();
		WrapperRHI::RHI::UseShader(m_pPostprocessShader->GetShaderProgram());
		WrapperRHI::RHI::ActivateAndBindTexture2D(0, m_colorBuffers[0]);
		WrapperRHI::RHI::ActivateAndBindTexture2D(1, m_pingpongBuffers[!horizontal]);
		WrapperRHI::RHI::Send1fToShader(m_pPostprocessShader, "exposure", 0.5f);
		RenderToScreenQuad();
		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::UIPass(std::map<std::string, SceneManagement::GameObject*> gameObjects)
	{
		Matrix4D projection = Matrix4D::OrthographicMatrix(0.f, m_width, 0.f, m_height);
		WrapperRHI::RHI::SetActiveFramebuffer(m_colorBuffer);

		for (std::map<std::string, SceneManagement::GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			if (it->second->HasComponent<UI::Text>())
			{
				UI::Text* text = it->second->GetComponent<UI::Text>();
				if (!text->isEnable) continue;

				if (text->GetFont() != nullptr && text->GetFont()->IsCreated())
				{
					WrapperRHI::RHI::UseShader(m_pUITextShader->GetShaderProgram());
					WrapperRHI::RHI::SendMat4ToShader(m_pUITextShader, "projection", 1, false, &projection.e[0]);
					WrapperRHI::RHI::DrawTextUI(m_pUITextShader, text->GetText(), text->GetFont(), Vector2D(text->GetPosition().x * m_width, text->GetPosition().y* m_height), text->GetSize(), text->GetColor(), m_UIVAO, m_UIVBO);
				}
			}

			if (it->second->HasComponent<UI::Image>())
			{
				UI::Image* image = it->second->GetComponent<UI::Image>();
				if (!image->isEnable) continue;

				
				if (image->GetTexture() != nullptr && image->GetTexture()->IsCreated())
				{
					WrapperRHI::RHI::EnableOptions(BLEND);
					WrapperRHI::RHI::SetBlendFunc(SRC_ALPHA,ONE_MINUS_SRC_ALPHA);

					WrapperRHI::RHI::UseShader(m_pUIImageShader->GetShaderProgram());
					WrapperRHI::RHI::SendMat4ToShader(m_pUIImageShader, "projection", 1, false, &projection.e[0]);
					WrapperRHI::RHI::SendMat4ToShader(m_pUIImageShader, "model", 1, false, &image->GetModelMatrix().e[0]);
					WrapperRHI::RHI::Send1fToShader(m_pUIImageShader, "opacity",  image->opacity);
					WrapperRHI::RHI::DrawImageUI(image->GetTexture()->GetTexture(), m_UIImageVAO, m_UIImageVBO);

					WrapperRHI::RHI::DisableOptions(BLEND);
				}
			}
		}

		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::RenderToScreenQuad()
	{
		WrapperRHI::RHI::DrawScreenQuad(m_quadVAO);
	}

	void RenderManager::GenerateShadowMap(Resources::Shader* shader, LightSystem::Light* light, std::vector<SceneManagement::ObjInfo*> objects, Physics::Transform* cameraTransform)
	{
		WrapperRHI::RHI::SetActiveFramebuffer(light->GetShadowMapFBO());
		WrapperRHI::RHI::ClearDepth();
		WrapperRHI::RHI::SetCullFace(FRONT);

		WrapperRHI::RHI::UseShader(shader->GetShaderProgram());
		light->SendLightToShadowShader(shader);

		for (std::vector<SceneManagement::ObjInfo*>::iterator it = objects.begin(); it != objects.end(); ++it)
				WrapperRHI::RHI::DrawMeshShadowPass((*it),shader);

		WrapperRHI::RHI::SetCullFace(BACK);
		WrapperRHI::RHI::SetActiveFramebuffer(0);
	}

	void RenderManager::SendShadowMapToShader(LightSystem::LightManager* lightManager, Physics::Transform* cameraTransform, int indexStart)
	{
		int nbPointLight = 0, nbDirLight = 0, nbSpotLight = 0;
		int nbPointLightMax = 7, nbDirLightMax = 4, nbSpotLightMax = 4;

		int startIndexPointLight = indexStart, startIndexDirLight = indexStart + 7, startIndexSpotLight = indexStart + 11;

		for (int i = 0; i < lightManager->GetLightNumber(); i++)
		{
			if (!lightManager->GetLight(i)->IsGeneratingShadow() || !lightManager->GetLight(i)->isEnable)
				continue;

			if ((lightManager->GetLight(i)->GetType() != LightSystem::TypeLight::DIR_LIGHT) && (lightManager->GetLight(i)->GetGameObject()->GetTransform()->GetGlobalPosition() - cameraTransform->GetGlobalPosition()).Magnitude() >= distanceCullingLights)
				continue;

			LightSystem::TypeLight type = lightManager->GetLight(i)->GetType();
			if (type == LightSystem::TypeLight::POINT_LIGHT && nbPointLight < nbPointLightMax)
			{
				WrapperRHI::RHI::ActivateAndBindCubemap(startIndexPointLight + nbPointLight, lightManager->GetLight(i)->GetShadowMap());
				nbPointLight++;
			}
			else if (type == LightSystem::TypeLight::SPOT_LIGHT && nbSpotLight < nbSpotLightMax)
			{
				WrapperRHI::RHI::ActivateAndBindTexture2D(startIndexSpotLight + nbSpotLight, lightManager->GetLight(i)->GetShadowMap());
				nbSpotLight++;
			}
			else if (type == LightSystem::TypeLight::DIR_LIGHT && nbDirLight < nbDirLightMax)
			{
				WrapperRHI::RHI::ActivateAndBindTexture2D(startIndexDirLight + nbDirLight, lightManager->GetLight(i)->GetShadowMap());
				nbDirLight++;
			}
		}
	}

	void RenderManager::Destroy()
	{
		WrapperRHI::RHI::DeleteTexture(m_ssaoColorBuffer);
		WrapperRHI::RHI::DeleteFramebuffer(m_ssaoFBO);
		WrapperRHI::RHI::DeleteTexture(m_ssaoColorBufferBlur);
		WrapperRHI::RHI::DeleteFramebuffer(m_ssaoBlurFBO);
		WrapperRHI::RHI::DeleteTexture(m_colorBuffers[0]);
		WrapperRHI::RHI::DeleteTexture(m_colorBuffers[1]);
		WrapperRHI::RHI::DeleteFramebuffer(m_hdrFBO);
		WrapperRHI::RHI::DeleteRenderbuffer(m_rboDepth);
		WrapperRHI::RHI::DeleteTexture(m_pingpongBuffers[0]);
		WrapperRHI::RHI::DeleteTexture(m_pingpongBuffers[1]);
		WrapperRHI::RHI::DeleteFramebuffer(m_pingpongBuffers[0]);
		WrapperRHI::RHI::DeleteFramebuffer(m_pingpongBuffers[1]);
		WrapperRHI::RHI::DeleteTexture(m_colorTexture);
		WrapperRHI::RHI::DeleteFramebuffer(m_colorBuffer);
		WrapperRHI::RHI::DeleteTexture(m_pickingTexture);
		WrapperRHI::RHI::DeleteFramebuffer(m_pickingBuffer);
		WrapperRHI::RHI::DeleteVertexArray(m_quadVAO);
		WrapperRHI::RHI::DeleteBuffer(m_quadVBO);
		WrapperRHI::RHI::DeleteVertexArray(m_UIVAO);
		WrapperRHI::RHI::DeleteBuffer(m_UIVBO);
		WrapperRHI::RHI::DeleteVertexArray(m_UIImageVAO);
		WrapperRHI::RHI::DeleteBuffer(m_UIImageVBO);

		m_pGBuffer->Destroy();
		m_pKernels->Destroy();
		delete m_pGBuffer;
	}
}