#pragma once
#include <Define.h>
#include <Resources/Texture.h>
#include <Scene/Component.h>
#include <Scene/GameObject.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Reflexion/VectorReflexionProperty.h>
#include <Generated/Image.rfkh.h>

namespace UI NAMESPACE()
{
	class CLASS()  LICH_ENGINE Image : public SceneManagement::Component
	{
	public:

		Image();
		Image(SceneManagement::GameObject* gO);
		~Image();

		void Update(Physics::Transform& transform) override;
		void LoadData(std::string data) override;
		std::string SaveData() override;
		void ApplyEditorChange() override;

		
		void SetImage(Vector2D position, float rotation, Vector2D scale, Resources::Texture* texture);

		inline void SetTexture(Resources::Texture* texture) { m_texture = texture; }
		inline void SetRotation(float rotation) { m_rotation = rotation; }
		inline void SetPosition(Vector2D position) { m_position = position; }
		inline void SetScale(Vector2D scale) { m_scale = scale; }

		inline Resources::Texture* GetTexture() { return m_texture; }
		inline float GetRotation() { return m_rotation; }
		inline Vector2D GetPosition() { return m_position; }
		inline Vector2D GetScale() { return m_scale; }
		inline Matrix4D GetModelMatrix() { return m_modelMatrix; }

		FIELD() float opacity = 1;

	private:

		FIELD (ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Resources::Texture* m_texture;
		FIELD () float m_rotation;
		FIELD(RangeProperty(0,1)) Vector2D m_position; //[0,1] [0,1]
		FIELD() Vector2D m_scale;
		Matrix4D m_modelMatrix;

		UI_Image_GENERATED
	};
}
File_Image_GENERATED