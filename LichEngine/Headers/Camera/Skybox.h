#pragma once
#include <vector>

#include <Define.h>
#include <Resources/Model.h>
#include <Resources/Texture.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Skybox.rfkh.h>

namespace Resource
{
	class Texture;
}

enum SkyboxFace
{
	RIGHTFACE,
	LEFTFACE,
	TOPFACE,
	BOTTOMFACE,
	FRONTFACE,
	BACKFACE,
};

namespace CameraRender NAMESPACE()
{
	class CLASS() LICH_ENGINE SkyBox : public Resources::Resource
	{
	private:

		//Textures can be change in the editor
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture * m_top = nullptr;
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture* m_bottom  = nullptr;
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture* m_right = nullptr;
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture* m_left = nullptr;
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture* m_front = nullptr;
	FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE))	Resources::Texture* m_back = nullptr;

		Resources::Model* m_model; //Always the same cube model
		std::vector<Resources::Resource*> m_faces;
		std::vector<std::filesystem::path> m_pathFaces;
		uint m_cubemap;
		bool textureSet;


	public:

		SkyBox(const char* name, const char* path);
		~SkyBox();

		void Create();
		void GetResources();
		void UpdateCubemap();
		void SetTextureInSkybox(Resources::Texture * texture, SkyboxFace face);
		Resources::Texture* GetTextureInSkybox(SkyboxFace face);
		void SaveInSkyboxFile();

		inline Resources::Model* GetModel() { return m_model; }
		inline uint& GetCubeMap() { return m_cubemap; }
		inline std::vector<Resources::Resource*> GetTexturesResources() { return m_faces; }

		void Load(const std::string & path) override;
		std::string Save() override;
		bool IsCreated() override;
		void Unload() override;

		CameraRender_SkyBox_GENERATED
	};
}
File_Skybox_GENERATED