#pragma once
#include <Resources/Shader.h>
#include <Scene/Component.h>
#include <Scene/GameObject.h>
#include <Generated/Light.rfkh.h>


namespace LightSystem NAMESPACE()
{
	enum class ENUM() TypeLight
	{
		DIR_LIGHT = 0,
		SPOT_LIGHT = 1,
		POINT_LIGHT = 2
	};

	class CLASS() LICH_ENGINE Light : public SceneManagement::Component
	{
	protected:
		FIELD()  bool p_generateShadow;
		FIELD() float p_intensity = 1;
		FIELD() float p_ambient = 0.04f;
		FIELD() Vector4D p_color;
		Vector3D p_position = Vector3D();
		FIELD() TypeLight p_type;
		TypeLight p_prevType;
		Vector2D p_sizeShadowMap;
		uint p_shadowMapFBO;
		uint p_shadowMap;

		void ChangeType(SceneManagement::GameObject * objOwner);

	public:
		Light();
		~Light();

		void SetType(const TypeLight type);
		void SetGenerateShadow(const bool shadow);
		void SetLightColor(const Vector4D color);
		void SetPosition(const Vector3D position) { p_position = position; }
		void SetIntensity(const float intensity) { p_intensity = intensity; }

		virtual void SendLightToShader(int index, Resources::Shader * shaderProgram) = 0;
		virtual void SendLightToShadowShader(Resources::Shader* shaderProgram) = 0;


		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		std::string SaveData() override;
		void ApplyEditorChange() override;

		inline bool IsGeneratingShadow() { return p_generateShadow; }
		inline Vector3D GetColor() { return p_color; }
		inline Vector2D GetSizeShadowMap() { return p_sizeShadowMap; }
		inline TypeLight GetType() { return p_prevType; }
		inline uint GetShadowMap() { return p_shadowMap; }
		inline uint GetShadowMapFBO() { return p_shadowMapFBO; }
		inline float GetIntensity() { return p_intensity; }


	LightSystem_Light_GENERATED
	};
};
File_Light_GENERATED