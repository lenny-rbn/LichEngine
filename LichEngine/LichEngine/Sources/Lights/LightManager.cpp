#include <pch.h>
#include <Core/LightManager.h>
#include <Core/DirLight.h>
#include <Core/SpotLight.h>
#include <Core/PointLight.h>
#include <ToolBox.h>

using namespace LightSystem;

bool LightSystem::LightManager::CheckLightActive(Light* light,int& count)
{
	if (!light->IsEnableChange()) return true;
	
	light->ResetEnableTransition();
	
	if (!light->isEnable)
	{
		count--;
		return false;
	}
	else
	{
		count++;
		
		return true;
	}
}

LightManager::LightManager()
{
	m_dirLightNB = 0;
	m_spotLightNB = 0;
	m_pointLightNB = 0;
}

LightManager::~LightManager() {}

void LightManager::Update(Resources::Shader* shaderProgram, Physics::Transform* cameraTransform, float distanceCulling)
{
	int dir = 0,spot = 0,point = 0;

	for (int i = 0; i < m_lights.size(); i++)
	{
		if (!m_lights[i]->isEnable && !m_lights[i]->IsEnableChange()) continue;

		if ((m_lights[i]->GetType() != LightSystem::TypeLight::DIR_LIGHT) && (m_lights[i]->GetGameObject()->GetTransform()->GetGlobalPosition() - cameraTransform->GetGlobalPosition()).Magnitude() >= distanceCulling)
			continue;

		switch (m_lights[i]->GetType())
		{
		case TypeLight::DIR_LIGHT:
			
			if (!CheckLightActive(m_lights[i],m_dirLightActiveNB)) continue;

			m_lights[i]->SendLightToShader(dir,shaderProgram);
			dir++;
			break;
		case TypeLight::SPOT_LIGHT:
			if (!CheckLightActive(m_lights[i],m_spotLightActiveNB)) continue;

			m_lights[i]->SendLightToShader(spot,shaderProgram);
			spot++;
			break;
		case TypeLight::POINT_LIGHT:
			
			if (!CheckLightActive(m_lights[i],m_pointLightActiveNB)) continue;

			m_lights[i]->SendLightToShader(point,shaderProgram);
			point++;
			break;
		default:
			break;
		}
	}

	WrapperRHI::RHI::Send1iToShader(shaderProgram,"nbDirLights", dir);
	WrapperRHI::RHI::Send1iToShader(shaderProgram,"nbSpotLights", spot);
	WrapperRHI::RHI::Send1iToShader(shaderProgram,"nbPointLights", point);
}

void LightSystem::LightManager::AddLight(LightSystem::Light* lightAdd)
{
	m_lights.push_back(lightAdd);
	if (lightAdd->GetType() == TypeLight::DIR_LIGHT)
	{
		m_dirLightNB++;
		m_dirLightActiveNB++;
	}
	if (lightAdd->GetType() == TypeLight::POINT_LIGHT)
	{
		m_pointLightNB++;
		m_pointLightActiveNB++;
	}
	if (lightAdd->GetType() == TypeLight::SPOT_LIGHT)
	{
		m_spotLightNB++;
		m_spotLightActiveNB++;
	}
}

void LightSystem::LightManager::RemoveLight(LightSystem::Light* lightRemove)
{

	if (lightRemove->GetType() == TypeLight::DIR_LIGHT)
	{
		if (lightRemove->isEnable)	m_dirLightActiveNB--;
		m_dirLightNB--;
	}
	if (lightRemove->GetType() == TypeLight::POINT_LIGHT)
	{
		m_pointLightNB--;
		if (lightRemove->isEnable) m_pointLightActiveNB--;
	}
	if (lightRemove->GetType() == TypeLight::SPOT_LIGHT)
	{
		m_spotLightNB--;
		if (lightRemove->isEnable) m_spotLightActiveNB--;
	}
	if (Utils::IsInVector< LightSystem::Light* >(m_lights,lightRemove))
	{
		Utils::EraseVectorElement< LightSystem::Light* >(m_lights,lightRemove);
	}
}

