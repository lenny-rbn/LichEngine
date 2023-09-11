#include <pch.h>
#include <Core/TimeManager.h>
#include <Scene/GameObject.h>
#include <Scene/Scene.h>
#include <Physics/Transform.h>
#include <Resources/ResourceManager.h>
#include <Generated/Camera.rfks.h>


CameraRender::Camera::Camera(SceneManagement::GameObject* gO)
{
	p_pGameObject = gO;
	m_direction = Vector3D(0.f, 0.f, -1.f);
	m_settings.aspectRatio = 1920.f / 1080.f;
	m_settings.fov = 60.f;
	m_settings.cameraFar = 100.f;
	m_settings.cameraNear = 0.01f;
	m_pSkyBox = nullptr;
	componentType = SceneManagement::ComponentType::CAMERA;
}

CameraRender::Camera::~Camera()
{
	m_pSkyBox = nullptr;
}

void CameraRender::Camera::CalculateViewMatrix(Physics::Transform& transform)
{
	m_viewMatrix = Matrix4D::LookAt(transform.GetGlobalPosition(), transform.GetGlobalPosition() + m_direction, Vector3D::up);
}

void CameraRender::Camera::CalculateProjectionMatrix()
{
	m_projectionMatrix = Matrix4D::ProjectionMatrix(m_settings.fov, m_settings.aspectRatio, m_settings.cameraNear, m_settings.cameraFar);
}

Vector3D CameraRender::Camera::GetRayFromScreenCoord(Vector2D coord)
{
	Matrix4D invMat = Matrix4D::InverseMatrix(m_projectionMatrix * m_viewMatrix);
	Vector4D nearVec = Vector4D((coord.x - 860.f) / 860.f, -1 * (coord.y - 540.f) / 540.f, m_settings.cameraNear, 1.0);
	Vector4D farVec = Vector4D((coord.x - 860.f) / 860.f, -1 * (coord.y - 540.f) / 540.f, m_settings.cameraFar, 1.0);
	Vector4D nearResult = invMat * nearVec;
	Vector4D farResult = invMat * farVec;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	Vector3D dir = farResult - nearResult;
	return Vector3D::Normalize(dir);
}

void CameraRender::Camera::CalculateMatrix()
{
	CalculateViewMatrix((*p_pGameObject->GetTransform()));
	CalculateProjectionMatrix();
}

void CameraRender::Camera::SetScreenSize(Vector2D size)
{

	m_settings.aspectRatio = size.x / size.y;
}

void CameraRender::Camera::Update(Physics::Transform& transform)
{
	m_direction =  transform.GetVectorForward();
}

Matrix4D CameraRender::Camera::GetVPMatrix()
{
	return  m_projectionMatrix * m_viewMatrix;
}

Vector2D CameraRender::Camera::WorldToScreenCoords(Vector3D coords)
{
	Vector4D postProjectivePosition = m_projectionMatrix * m_viewMatrix * Vector4D(coords, 1.0f);
	float clipSpaceX = postProjectivePosition.x / abs(postProjectivePosition.w);
	float clipSpaceY = postProjectivePosition.y / abs(postProjectivePosition.w);
	std::cout << postProjectivePosition.w << "\n";
	return Vector2D(clipSpaceX, clipSpaceY);
}

void CameraRender::Camera::LoadData(std::string data)
{
	const int sizeData = 6;
	std::string dataCut[sizeData];
	size_t index = 0;
	size_t baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', index + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	m_settings.fov = std::stof(dataCut[0]);
	m_settings.aspectRatio = std::stof(dataCut[1]);
	m_settings.cameraFar = std::stof(dataCut[2]);
	m_settings.cameraNear = std::stof(dataCut[3]);
	m_direction.FromString(dataCut[4]);
	SetSkybox(dataCut[5]);
}

Matrix4D CameraRender::Camera::GetViewMatrix()
{
	return  m_viewMatrix;
}

std::string CameraRender::Camera::SaveData()
{
	std::string content = "~0,";
	content += std::to_string(m_settings.fov) + ',';
	content += std::to_string(m_settings.aspectRatio) + ',';
	content += std::to_string(m_settings.cameraFar) + ',';
	content += std::to_string(m_settings.cameraNear) + ',';
	content += m_direction.ToString() + ',';
	if(m_pSkyBox)
		content += m_pSkyBox->name + ',';
	else
		content += "..,";

	return content;
}

void CameraRender::Camera::SetSkybox(SkyBox* skyB)
{
	m_pSkyBox = skyB;
	if (isMainCamera)
		 p_pGameObject->GetSceneGraph()->GetScene()->SetSkybox(skyB);
}

void CameraRender::Camera::SetSkybox(const std::string skyboxName)
{
	if (skyboxName != "..")
		m_pSkyBox = Resources::ResourceManager::GetResource<SkyBox>(skyboxName);
}

void CameraRender::Camera::ApplyEditorChange()
{

}