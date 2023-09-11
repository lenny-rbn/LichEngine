#pragma once
#include <Core/Light.h>


namespace LightSystem 
{
	class  LICH_ENGINE DirLight : public Light
	{
	private:
		Vector3D m_direction;

		Matrix4D m_view;
		Matrix4D m_projection;
		Matrix4D m_lightSpaceMatrix;

	public:
		DirLight(SceneManagement::GameObject* gO);
		DirLight(SceneManagement::GameObject* gO, Vector3D direction);
		~DirLight();

		void SetLight(Vector3D direction);
	
		inline Matrix4D GetLightSpaceMatrix() { return m_lightSpaceMatrix; }

		std::string SaveData() override;
		void ApplyEditorChange() override;
		void LoadData(std::string data) override;
		void Update(Physics::Transform& transform) override;
		void SendLightToShader(int index, Resources::Shader* shaderProgram) override;
		void SendLightToShadowShader(Resources::Shader* shaderProgram) override;
	};
}