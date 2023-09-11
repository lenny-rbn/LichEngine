#pragma once
#include <Maths.h>
#include <Camera/Skybox.h>
#include <Scene/Component.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Camera.rfkh.h>

namespace SceneManagement
{
	class GameObject;
}

namespace CameraRender NAMESPACE()
{
	struct STRUCT() LICH_ENGINE CameraSettings
	{
		float aspectRatio = 0.f;
		FIELD() float cameraNear = 0.f;
		FIELD() float cameraFar = 0.f;

		FIELD() float fov = 0.f;
		Vector2D viewportSize;

		float windowHalfWidth = 0.f;
		float windowHalfHeight = 0.f;
		CameraRender_CameraSettings_GENERATED;
	};

	class CLASS() LICH_ENGINE  Camera : public SceneManagement::Component
	{
	private:
		Vector3D m_direction;
		Matrix4D m_projectionMatrix;
		Matrix4D m_viewMatrix;
		
		FIELD() CameraSettings m_settings;
		FIELD(ResourceTypeProperty(Resources::ResourceType::SKYBOX)) SkyBox* m_pSkyBox;

		void CalculateProjectionMatrix();
		void CalculateViewMatrix(Physics::Transform & transform);


	public:
		Camera(SceneManagement::GameObject* gO);
		~Camera();

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		void SetPerspectiveView();
		void SetOrthographicView();
		void SetSkybox(SkyBox * skyB);
		void SetSkybox(const std::string name);
		void SetScreenSize(Vector2D size);
		void Create(const CameraSettings & set);
		void CalculateMatrix();
		Vector3D GetRayFromScreenCoord(Vector2D coord);
		inline SkyBox* GetSkyBox() { return m_pSkyBox; }
		Vector2D WorldToScreenCoords(Vector3D coors);

		Matrix4D GetVPMatrix();
		Matrix4D GetViewMatrix();

		bool isMainCamera =false;
		Matrix4D GetProjection() { return m_projectionMatrix; }
		CameraRender_Camera_GENERATED
	};
};
File_Camera_GENERATED