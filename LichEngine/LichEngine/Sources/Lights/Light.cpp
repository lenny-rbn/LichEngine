#include <pch.h>
#include <Physics/Transform.h>
#include <Scene/GameObject.h>
#include <Core/DirLight.h>	
#include <Core/PointLight.h>	
#include <Core/SpotLight.h>	
#include <Generated/Light.rfks.h>
using namespace LightSystem;

Light::Light()
{
	p_generateShadow = true;
	p_sizeShadowMap = Vector2D(2048, 2048);
}

Light::~Light() 
{
	WrapperRHI::RHI::DeleteTexture(p_shadowMap);
	WrapperRHI::RHI::DeleteFramebuffer(p_shadowMapFBO);
}

void Light::SetType(const TypeLight type)
{
	this->p_type = type;
}

void Light::SetGenerateShadow(const bool shadow)
{
	this->p_generateShadow = shadow;
}

void Light::SetLightColor(const Vector4D color)
{
	this->p_color = color;
}

void Light::Update( Physics::Transform& transform)
{

}

void Light::LoadData(std::string data)
{

}

void Light::ApplyEditorChange()
{
	ChangeEnableState();
}

std::string Light::SaveData()
{
	std::string content = "~Light,";
	content += GetColor().ToString() + ',';
	return std::string();
}

void Light::ChangeType(SceneManagement::GameObject* objOwner)
{
	if (p_type == LightSystem::TypeLight::DIR_LIGHT) objOwner->AddComponent<LightSystem::DirLight>();
	if (p_type == LightSystem::TypeLight::SPOT_LIGHT) objOwner->AddComponent<LightSystem::SpotLight>();
	if (p_type == LightSystem::TypeLight::POINT_LIGHT) objOwner->AddComponent<LightSystem::PointLight>();

	if(p_prevType == LightSystem::TypeLight::DIR_LIGHT) objOwner->RemoveComponent<LightSystem::DirLight>(componentType);
	if(p_prevType == LightSystem::TypeLight::SPOT_LIGHT) objOwner->RemoveComponent<LightSystem::SpotLight>(componentType);
	if(p_prevType == LightSystem::TypeLight::POINT_LIGHT) objOwner->RemoveComponent<LightSystem::PointLight>(componentType);
	
}
