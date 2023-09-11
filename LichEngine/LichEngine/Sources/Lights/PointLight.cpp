#include <pch.h>
#include <Physics/Transform.h>
#include <Generated/PointLight.rfks.h>

using namespace LightSystem;

PointLight::PointLight(SceneManagement::GameObject* gO)
{
	p_pGameObject = gO;
	this->m_linear = 0.1f;
	this->m_constant = 0.4f;
	this->m_quadratic = 0.32f;
	m_projection = Matrix4D::ProjectionMatrix(90.f, 1.f, 0.1f, 30.f);
	p_type = TypeLight::POINT_LIGHT;
	p_prevType = TypeLight::POINT_LIGHT;

	for (int i = 0; i < 6; i++)
		m_lightSpaceMats.push_back(Matrix4D());
	
	WrapperRHI::RHI::CreateShadowFBOandCubeMap(p_shadowMapFBO, p_shadowMap, GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::POINT_LIGHT;
}

PointLight::PointLight(SceneManagement::GameObject* gO, Vector3D position, float linear, float constant, float quadratic)
{
	p_pGameObject = gO;
	this->p_position = position;
	this->m_linear = linear;
	this->m_constant = constant;
	this->m_quadratic = quadratic;

	m_projection = Matrix4D::ProjectionMatrix(90.f, 1.f, 0.1f, 50.f);

	for (int i = 0; i < 6; i++)
		m_lightSpaceMats.push_back(Matrix4D());

	WrapperRHI::RHI::CreateShadowFBOandCubeMap(p_shadowMapFBO, p_shadowMap, GetSizeShadowMap());
	componentType = SceneManagement::ComponentType::POINT_LIGHT;
}

PointLight::~PointLight() 
{

}

void PointLight::SetLight(Vector3D position, float linear, float constant, float quadratic)
{
	this->p_position = position;
	this->m_linear = linear;
	this->m_constant = constant;
	this->m_quadratic = quadratic;
}

void LightSystem::PointLight::Update(Physics::Transform& transform)
{
	

	if (p_type != p_prevType) ChangeType(transform.GetGameObject());
	p_position = transform.GetGlobalPosition();
}

void PointLight::LoadData(std::string data)
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
	p_position.FromString(dataCut[3]);
	m_linear = std::stof(dataCut[4]);
	m_constant = std::stof(dataCut[5]);
	m_quadratic = std::stof(dataCut[6]);
	p_ambient = std::stof(dataCut[7]);
}

void PointLight::ApplyEditorChange()
{
}

std::string PointLight::SaveData()
{
	std::string content = "~2,";
	content += GetColor().ToString() + ',';
	content += std::to_string(p_intensity) + ',';
	content += std::to_string(p_generateShadow) + ',';
	content += p_position.ToString() + ',';
	content += std::to_string(m_linear) +',';
	content += std::to_string(m_constant) +',';
	content += std::to_string(m_quadratic) +',';
	content += std::to_string(p_ambient) + ',';
	return content;
}

void PointLight::SendLightToShader(int index, Resources::Shader* shaderProgram)
{
	std::string i = std::to_string(index);
	WrapperRHI::RHI::Send3fToShader(shaderProgram, ("pointLights[" + i + "].position").c_str(), p_position);
	WrapperRHI::RHI::Send3fToShader(shaderProgram, ("pointLights[" + i + "].diffuse").c_str(), GetColor() * p_intensity);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("pointLights[" + i + "].linear").c_str(), m_linear);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("pointLights[" + i + "].constant").c_str(), m_constant);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("pointLights[" + i + "].quadratic").c_str(), m_quadratic);
	WrapperRHI::RHI::Send1iToShader(shaderProgram, ("pointLights[" + i + "].shadow").c_str(), p_generateShadow);
	WrapperRHI::RHI::Send1fToShader(shaderProgram, ("pointLights[" + i + "].ambient").c_str(), p_ambient);
}

void PointLight::SendLightToShadowShader(Resources::Shader* shaderProgram)
{
	m_lightSpaceMats[0] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::right, Vector3D::down);
	m_lightSpaceMats[1] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::left, Vector3D::down);
	m_lightSpaceMats[2] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::up, Vector3D::forward);
	m_lightSpaceMats[3] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::down, Vector3D::back);
	m_lightSpaceMats[4] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::forward, Vector3D::down);
	m_lightSpaceMats[5] = m_projection * Matrix4D::LookAt(p_position, p_position + Vector3D::back, Vector3D::down);

	for (unsigned int i = 0; i < 6; i++)
		WrapperRHI::RHI::SendMat4ToShader(shaderProgram, ("shadowMatrices[" + std::to_string(i) + "]").c_str(), 1, false, &m_lightSpaceMats[i].e[0]);

	WrapperRHI::RHI::Send1fToShader(shaderProgram, "far_plane", 30.f);
	WrapperRHI::RHI::Send3fToShader(shaderProgram, "lightPos", p_position);
}