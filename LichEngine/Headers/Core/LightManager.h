#pragma once
#include <Core/Light.h>
#include <Core/Window.h>


namespace LightSystem
{
	class LICH_ENGINE LightManager
	{
	private:
		std::vector<Light*> m_lights;

		int m_dirLightNB,m_spotLightNB,m_pointLightNB;
		int m_dirLightActiveNB,m_spotLightActiveNB,m_pointLightActiveNB;

		bool CheckLightActive(Light* light,int& count);

	public:
		LightManager();
		~LightManager();

		void AddLight(LightSystem::Light* lightAdd);
		void Update(Resources::Shader* shaderProgram, Physics::Transform* cameraTransform, float distanceCulling);
		void RemoveLight(LightSystem::Light* lightRemove);
		
		int GetDirLightNB() { return m_dirLightNB; }
		int GetSpotLightNB() { return m_spotLightNB; }
		int GetPointLightNB() { return m_pointLightNB; }
		int GetLightNumber() { return (int)m_lights.size(); }

		Light* GetLight(int index) { return m_lights[index]; }
	};
}