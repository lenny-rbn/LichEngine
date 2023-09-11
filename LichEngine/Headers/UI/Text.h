#pragma once
#include <Define.h>
#include <UI/Font.h>
#include <Scene/Component.h>
#include <Reflexion/VectorReflexionProperty.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Text.rfkh.h>

namespace UI NAMESPACE()
{
	class CLASS() LICH_ENGINE Text : public SceneManagement::Component
	{
	public:

		Text();
		Text(SceneManagement::GameObject * gO);
		~Text();

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		void SetTextParam(std::string text, Font * font, Vector2D pos, float size, Vector4D color);
		void SetFont(std::string fontName);
		void SetText(std::string text);

		inline void SetFont(Font * font) { m_pFont = font; }
		inline void SetSize(float size) { m_size = size; }
		inline void SetPosition(Vector2D pos) { m_position = pos; }
		inline void SetColor(Vector3D color) { m_color = color; }
		inline void SetOpacity(float opacity) { m_color.w = opacity; }


		inline Font* GetFont() { return m_pFont; }
		inline float GetSize() { return m_size; }
		inline Vector2D GetPosition() { return m_position; }
		inline Vector4D GetColor() { return m_color; }
		inline std::string GetText() { return m_text; }

	private:

		FIELD() const char* m_text;
		std::string m_textString;

		FIELD() float m_size;
		FIELD(RangeProperty(0.0f,1.0f)) Vector2D m_position; //[0,1] [0,1]
		FIELD() Vector4D m_color;
		FIELD(ResourceTypeProperty(Resources::ResourceType::FONT)) Font* m_pFont;
		UI_Text_GENERATED
	};
}

File_Text_GENERATED

