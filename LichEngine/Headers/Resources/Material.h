#pragma once
#include <Resources/Texture.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Material.rfkh.h>


namespace Resources NAMESPACE()
{
	class CLASS() LICH_ENGINE Material : public Resource
	{
	public:
		Material();
		Material(const char* name,const char* path);
		~Material();


		void Load(const std::string & path) override;
		std::string Save() override;
		bool IsCreated() override;
		void Unload() override;
		void SaveInMatFile();

		inline Texture* GetDiffuseMap() { return m_pDiffuseMap; }
		inline Texture* GetNormalMap() { return m_pNormalMap; }
		inline Texture* GetHeightMap() { return m_pHeightMap; }
		inline Texture* GetMetallicMap() { return m_pMetallicMap; }
		inline Texture* GetRoughnessMap() { return m_pRoughnessMap; }
		inline Texture* GetAOMap() { return m_pAoMap; }
		inline bool IsBloomActive() { return m_bloom; }
		inline bool IsGeneratingShadow() { return m_generateShadow; }
		inline bool IsUsingDiffuseTextForOpacity() { return m_useDiffuseTextForOpacity; }
		inline float GetOpacity() { return m_opacity; }
		inline float GetRoughness() { return m_roughness; }
		inline float GetMetalness() { return m_metalness; }
		inline Vector4D GetAlbedo() { return m_albedo; }
		inline std::vector<Resources::Texture*> GetTextures() { return m_pTextures; }

		inline void SetDiffuse(Texture* diffuse) { m_pDiffuseMap = diffuse; }
		inline void SetNormal(Texture* normal) { m_pNormalMap = normal; }
		inline void SetHeight(Texture* height) { m_pHeightMap = height; }
		inline void SetMetallic(Texture* metallic) { m_pMetallicMap = metallic; }
		inline void SetRouhgness(Texture* roughness) { m_pRoughnessMap = roughness; }
		inline void SetAO(Texture* AO) { m_pAoMap = AO; }

		void DeleteTextures();

	private:
		FIELD() bool m_bloom;
		FIELD() bool m_generateShadow;
		FIELD() bool m_useDiffuseTextForOpacity;

		FIELD() float m_opacity;   // [0;1]
		FIELD() float m_metalness; // [0;1]
		FIELD() float m_roughness; // [0;1]

		FIELD() Vector4D m_albedo; // [0;1] [0;1] [0;1]

		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pDiffuseMap;
		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pNormalMap;
		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pHeightMap;
		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pMetallicMap;
		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pRoughnessMap;
		FIELD(ResourceTypeProperty(Resources::ResourceType::TEXTURE)) Texture* m_pAoMap;

		std::string GetTexturePathToSave(Texture* texture);
		void GetTexturesFromResourcesManager();
		Texture* SetTexture(std::string path, bool gammaCorrection);
		void DeleteTexture(Texture* text);

		std::vector<Resources::Texture*> m_pTextures;
		std::vector<std::string> m_pathTextures;
		Texture* m_pOldDiffuse;

		Resources_Material_GENERATED
	};
};
File_Material_GENERATED