#include <pch.h>

#include <Resources/ResourceManager.h>
#include <Generated/Text.rfks.h>

namespace UI
{
	Text::Text()
	{
		m_text = "";
		m_pFont = nullptr;
		m_position = Vector2D(0, 0);
		m_size = 0;
		m_color = Vector4D(0, 0, 0);
	}

	Text::Text(SceneManagement::GameObject* gO)
	{
		m_text = "";
		m_pFont = nullptr;
		m_position = Vector2D(0, 0);
		m_size = 0;
		m_color = Vector4D(0, 0, 0);
		p_pGameObject = gO;
		componentType = SceneManagement::ComponentType::TEXT;
	}

	Text::~Text()
	{

	}

	void Text::SetTextParam(std::string text, Font* font, Vector2D pos, float size, Vector4D color)
	{
		m_textString = text;
		m_text = m_textString.c_str();
		m_pFont = font;
		m_position = pos;
		m_size = size;
		m_color = color;
	}

	void Text::SetFont(std::string fontName)
	{
		m_pFont = Resources::ResourceManager::GetResource<UI::Font>(fontName);
	}

	void Text::SetText(std::string text)
	{
		m_textString = text;
		m_text = m_textString.c_str();
	}

	void Text::Update(Physics::Transform& transform)
	{
		m_text = m_textString.c_str();
	}

	void Text::LoadData(std::string data)
	{
		const int sizeData = 5;
		std::string dataCut[sizeData];
		size_t index = 0;
		size_t baseIndex = 0;
		for (size_t i = 0; i < sizeData; i++)
		{
			index = data.find(',', index + 1);
			dataCut[i] = data.substr(baseIndex, index - baseIndex);
			baseIndex = index + 1;
		} 

		m_textString = dataCut[0];
		m_text = m_textString.c_str();
		m_size = std::stof(dataCut[1]);
		m_position.FromString(dataCut[2]);
		m_color = Vector3D::GetFromString(dataCut[3]);
		if (dataCut[4] != "none")
			m_pFont = Resources::ResourceManager::GetResource<UI::Font>(dataCut[4]);
		else
			m_pFont = nullptr;
	}

	void Text::ApplyEditorChange()
	{

	}

	std::string Text::SaveData()
	{
		std::string text(m_text);

		std::string content = "~11,";

		if (text.empty())
			text = " ";

		content += text + ',';
		content += std::to_string(m_size) + ',';
		content += m_position.ToString() + ',';
		content += m_color.ToString() + ',';
		if (m_pFont != nullptr)
			content += m_pFont->name + ',';
		else
			content += "none,";
		return content;
	}

}

