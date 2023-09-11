#include <pch.h>
#include <Core/DirLight.h>
#include <Physics/Transform.h>

using namespace LightSystem;

LightSystem::DirLight::DirLight(SceneManagement::GameObject* gO)
{
	p_pGameObject = gO;
	this->m_direction.z = 1;
	p_type = TypeLight::DIR_LIGHT;
	p_prevType = p_type;
	WrapperRHI::RHI::CreateShadowFBOandMap(p_shadowMapFBO,p_shadowMap,GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::DIR_LIGHT;
}

LightSystem::DirLight::DirLight(SceneManagement::GameObject* gO, Vector3D direction)
{
	p_pGameObject = gO;
	this->m_direction = direction;
	p_type = TypeLight::DIR_LIGHT;
	p_prevType = p_type;
	WrapperRHI::RHI::CreateShadowFBOandMap(p_shadowMapFBO,p_shadowMap,GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::DIR_LIGHT;
}

LightSystem::DirLight::~DirLight()
{

}

void LightSystem::DirLight::SetLight(Vector3D direction)
{
	this->m_direction = direction;
}

void LightSystem::DirLight::Update(Physics::Transform& transform)
{
	if (p_type != p_prevType) ChangeType(transform.GetGameObject());
	Vector3D rot = transform.GetGlobalRotation();
	m_direction = transform.GetVectorForward();
}

void DirLight::LoadData(std::string data)
{
	const int sizeData = 5;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;
	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',',baseIndex + 1);
		dataCut[i] = data.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}

	p_color = Vector3D::GetFromString(dataCut[0]);
	p_intensity = std::stof(dataCut[1]);
	p_generateShadow = std::stoi(dataCut[2]);
	m_direction = Vector3D::GetFromString(dataCut[3]);
	p_ambient = std::stof(dataCut[4]);
}

std::string DirLight::SaveData()
{
	std::string content = "~1,";
	content += GetColor().ToString() + ',';
	content += std::to_string(p_intensity) + ',';
	content += std::to_string(p_generateShadow) + ',';
	content += m_direction.ToString() + ',';
	content += std::to_string(p_ambient) + ',';
	return content;
}

void DirLight::ApplyEditorChange()
{
}

void DirLight::SendLightToShader(int index,Resources::Shader* shaderProgram)
{
	std::string i = std::to_string(index);

	WrapperRHI::RHI::Send3fToShader(shaderProgram,("dirLights[" + i + "].direction").c_str(),m_direction);
	WrapperRHI::RHI::Send3fToShader(shaderProgram,("dirLights[" + i + "].diffuse").c_str(),GetColor() * p_intensity);
	WrapperRHI::RHI::Send1iToShader(shaderProgram,("dirLights[" + i + "].shadow").c_str(), p_generateShadow);
	WrapperRHI::RHI::SendMat4ToShader(shaderProgram,("dirLights[" + i + "].lightSpaceMatrix").c_str(),1,false,&m_lightSpaceMatrix.e[0]);
	WrapperRHI::RHI::Send1fToShader(shaderProgram,("dirLights[" + i + "].ambient").c_str(), p_ambient);
}

void DirLight::SendLightToShadowShader(Resources::Shader* shadowShader)
{
	Vector3D pos = Vector3D(0) - m_direction * 20;
	m_view = Matrix4D::LookAt(pos, pos + m_direction, Vector3D(0, 1, 0));
	m_projection = Matrix4D::OrthographicMatrix(-30.f, 30.f, 30.f, -30.f, 1.f, 100.f);
	m_lightSpaceMatrix = m_projection * m_view;

	WrapperRHI::RHI::SendMat4ToShader(shadowShader, "lightSpaceMatrix", 1, false, &m_lightSpaceMatrix.e[0]);
}