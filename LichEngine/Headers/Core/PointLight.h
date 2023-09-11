#pragma once
#include <Core/Light.h>
#include <Generated/PointLight.rfkh.h>


namespace LightSystem NAMESPACE()
{
	class CLASS() LICH_ENGINE PointLight : public Light
	{
	private:
		float m_linear;
		float m_constant;
		float m_quadratic;

		Matrix4D m_projection;
		std::vector <Matrix4D> m_lightSpaceMats;

	public:
		PointLight(SceneManagement::GameObject * gO);
		PointLight(SceneManagement::GameObject * gO, Vector3D position, float linear, float constant, float quadratic);
		~PointLight();

		void SendDatasToShadowShader(Resources::Shader * shaderProgram);
		void SetLight(Vector3D position, float linear, float constant, float quadratic);

		void SendLightToShader(int index, Resources::Shader * shaderProgram) override;
		void SendLightToShadowShader(Resources::Shader * shaderProgram) override;

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		inline Matrix4D GetLightSpaceMatrix(int index) { return m_lightSpaceMats[index]; }

		LightSystem_PointLight_GENERATED
	};
};
File_PointLight_GENERATED