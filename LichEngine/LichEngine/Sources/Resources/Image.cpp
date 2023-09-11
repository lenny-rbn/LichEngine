#include <pch.h>
#include <Resources/ResourceManager.h>
#include <Renderer/RenderManager.h>
#include <Generated/Image.rfks.h>

namespace UI
{

	Image::Image()
	{

	}

	Image::Image(SceneManagement::GameObject* gO)
	{
		m_rotation = 0.f;
		m_position = Vector2D::zero;
		m_scale = Vector2D::zero;
		m_texture = nullptr;
		componentType = SceneManagement::ComponentType::IMAGE;
		p_pGameObject = gO;
	}

	Image::~Image()
	{

	}

	void Image::SetImage(Vector2D position, float rotation, Vector2D scale, Resources::Texture* texture)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
		m_texture = texture;
	}

	void Image::Update(Physics::Transform& transform)
	{
		Vector2D textScale = m_scale;
		if (m_texture != nullptr)
			textScale = Vector2D(m_texture->GetWidth() * m_scale.x, m_texture->GetHeight() * m_scale.y);

		Matrix4D translate = Matrix4D::CreateTranslationMatrix(Vector3D(m_position.x * Renderer::RenderManager::GetViewportSize().x, m_position.y * Renderer::RenderManager::GetViewportSize().y, 0));
		Matrix4D center = Matrix4D::CreateTranslationMatrix(Vector3D(0.5f * textScale.x, 0.5f * textScale.y, 0.0f));
		Matrix4D rotation = Matrix4D::CreateRotationMatrix(Vector3D(0.f, 0.f, m_rotation));
		Matrix4D uncenter = Matrix4D::CreateTranslationMatrix(Vector3D(-0.5f * textScale.x, -0.5f * textScale.y, 0.0f));
		Matrix4D scale = Matrix4D::CreateScaleMatrix(Vector3D(textScale.x, textScale.y, 1.0f));

		m_modelMatrix = translate * center * rotation * uncenter * scale;
	}

	void Image::LoadData(std::string data)
	{
		const int sizeData = 4;
		std::string dataCut[sizeData];
		size_t index = 0;
		size_t baseIndex = 0;
		for (size_t i = 0; i < sizeData; i++)
		{
			index = data.find(',', index + 1);
			dataCut[i] = data.substr(baseIndex, index - baseIndex);
			baseIndex = index + 1;
		}

		m_texture = Resources::ResourceManager::GetResource<Resources::Texture>(dataCut[0]);
		m_rotation = std::stof(dataCut[1]);
		m_position.FromString(dataCut[2]);
		m_scale.FromString(dataCut[3]);
	}

	std::string Image::SaveData()
	{
		std::string content = "~12,";
		content += m_texture->name + ',';
		content += std::to_string(m_rotation) + ',';
		content += m_position.ToString() + ',';
		content += m_scale.ToString() + ',';
		return content;
	}

	void Image::ApplyEditorChange()
	{

	}


}