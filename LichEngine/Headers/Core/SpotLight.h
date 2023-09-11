#pragma once
#include <Core/Light.h>
#include <Generated/SpotLight.rfkh.h>


namespace LightSystem NAMESPACE()
{
	class CLASS() LICH_ENGINE SpotLight : public Light
	{
	private:
		FIELD() float m_inerAngle;
		FIELD() float m_outerAngle;

		Vector3D m_direction;

		Matrix4D m_view;
		Matrix4D m_projection;
		Matrix4D m_lightSpaceMatrix;

	public:
		SpotLight(SceneManagement::GameObject * gO);
		SpotLight(SceneManagement::GameObject * gO, Vector3D position, Vector3D direction, float inerAngle, float outerAngle);
		~SpotLight();

		void SendDatasToShadowShader(Resources::Shader* shadowShader);
		void SetLight(Vector3D position, Vector3D direction, float inerAngle, float outerAngle);

		void SendLightToShader(int index, Resources::Shader* shaderProgram) override;
		void SendLightToShadowShader(Resources::Shader * shaderProgram) override;

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		inline Matrix4D GetLightSpaceMatrix() { return m_lightSpaceMatrix; }

		LightSystem_SpotLight_GENERATED
	};
};
File_SpotLight_GENERATED