#include <pch.h>
#include <Physics/Transform.h>
#include <Scene/GameObject.h>
#include <Generated/SpotLight.rfks.h>
using namespace LightSystem;

SpotLight::SpotLight(SceneManagement::GameObject* gO)
{
	p_pGameObject = gO;
	p_color = Vector4D(1, 1, 1);
	this->p_position = Vector3D::zero;
	this->m_direction = Vector3D::forward;
	this->m_inerAngle = 10.f;
	this->m_outerAngle = 20.f;
	p_type = TypeLight::SPOT_LIGHT;
	p_prevType = TypeLight::SPOT_LIGHT;
	WrapperRHI::RHI::CreateShadowFBOandMap(p_shadowMapFBO, p_shadowMap, GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::SPOT_LIGHT;
	m_view = Matrix4D::LookAt(p_position, p_position + m_direction, Vector3D(0, 1, 0));
	m_projection = Matrix4D::ProjectionMatrix(90.f, 1.f, 1.f, 100.f);
	m_lightSpaceMatrix = m_projection * m_view;

}

SpotLight::SpotLight(SceneManagement::GameObject* gO, Vector3D position, Vector3D direction, float inerAngle, float outerAngle)
{
	p_pGameObject = gO;
	p_color = Vector4D(1, 1, 1);
	this->p_position = position;
	this->m_direction = direction;
	this->m_inerAngle = inerAngle;
	this->m_outerAngle = outerAngle;
	WrapperRHI::RHI::CreateShadowFBOandMap(p_shadowMapFBO, p_shadowMap, GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::SPOT_LIGHT;
	m_view = Matrix4D::LookAt(p_position, p_position + m_direction, Vector3D(0, 1, 0));
	m_projection = Matrix4D::ProjectionMatrix(90.f, 1.f, 1.f, 100.f);
	m_lightSpaceMatrix = m_projection * m_view;

}

SpotLight::~SpotLight() 
{

}

void SpotLight::SetLight(Vector3D position, Vector3D direction, float inerAngle, float outerAngle)
{
	this->p_position = position;
	this->m_direction = direction;
	this->m_inerAngle = inerAngle;
	this->m_outerAngle = outerAngle;
}

void LightSystem::SpotLight::Update(Physics::Transform& transform)
{
	if (p_type != p_prevType) ChangeType(transform.GetGameObject());
	p_position = transform.GetGlobalPosition();
	m_direction = transform.GetVectorForward();
	m_view = Matrix4D::LookAt(p_position, p_position + m_direction, Vector3D(0, 1, 0));
	m_projection = Matrix4D::ProjectionMatrix(90.f, 1.f, 0.1f, 100.f);
	m_lightSpaceMatrix = m_projection * m_view;
}

void SpotLight::LoadData(std::string data)
{
	const int sizeData = 8;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	p_color = Vector3D::GetFromString(dataCut[0]);
	p_intensity = std::stof(dataCut[1]);
	p_generateShadow = std::stoi(dataCut[2]);
	m_direction.FromString(dataCut[3]);
	p_position.FromString(dataCut[4]);
	m_inerAngle = std::stof(dataCut[5]);
	m_outerAngle = std::stof(dataCut[6]);
	p_ambient = std::stof(dataCut[7]);
}

std::string SpotLight::SaveData()
{
	std::string content = "~3,";
	content += p_color.ToString() + ',';
	content += std::to_string(p_intensity) + ',';
	content += std::to_string(p_generateShadow) + ',';
	content += m_direction.ToString() + ',';
	content += p_position.ToString() + ',';
	content += std::to_string(m_inerAngle) + ',';
	content += std::to_string(m_outerAngle) + ',';
	content += std::to_string(p_ambient) + ',';
	return content;
}

void SpotLight::ApplyEditorChange()
{
}
void SpotLight::SendLightToShader(int index, Resources::Shader* shaderProgram)
{
	std::string i = std::to_string(index);

	WrapperRHI::RHI::Send3fToShader(shaderProgram, ("spotLights[" + i + "].position").c_str(), p_position);
	WrapperRHI::RHI::Send3fToShader(shaderProgram, ("spotLights[" + i + "].direction").c_str(), m_direction);
	WrapperRHI::RHI::Send3fToShader(shaderProgram, ("spotLights[" + i + "].diffuse").c_str(), GetColor() * p_intensity);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("spotLights[" + i + "].cosInerAngle").c_str(), cos(DEG2RAD(m_inerAngle)));
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("spotLights[" + i + "].cosOuterAngle").c_str(), cos(DEG2RAD(m_outerAngle)));
	WrapperRHI::RHI::Send1iToShader(shaderProgram, ("spotLights[" + i + "].shadow").c_str(), p_generateShadow);
	WrapperRHI::RHI::SendMat4ToShader(shaderProgram, ("spotLights[" + i + "].lightSpaceMatrix").c_str(), 1, false, &m_lightSpaceMatrix.e[0]);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("spotLights[" + i + "].ambient").c_str(), p_ambient);
}

void SpotLight::SendLightToShadowShader(Resources::Shader* shadowShader)
{
	WrapperRHI::RHI::SendMat4ToShader(shadowShader, "lightSpaceMatrix", 1, false, &m_lightSpaceMatrix.e[0]);
}