#include <pch.h>
#include <Resources/ResourceManager.h>
#include <Generated/Material.rfks.h>
#include <Toolbox.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <Debugs/Log.h>


namespace Resources
{
	Material::Material()
	{
		name = "None";
		m_pOldDiffuse = nullptr;
		m_pDiffuseMap = nullptr;
		m_pNormalMap = nullptr;
		m_pHeightMap = nullptr;
		m_pMetallicMap = nullptr;
		m_pRoughnessMap = nullptr;
		m_pAoMap = nullptr;
		m_bloom = false;
		m_generateShadow = true;
		m_useDiffuseTextForOpacity = false;
		m_opacity = 1.f;  
		m_metalness = 0.0f;
		m_roughness = 0.0f;
		m_albedo = Vector4D(255.f / 255.f, 62.f / 255.f, 209.f / 255.f, 0.8f);
		type = ResourceType::MATERIAL;
	}

	Material::Material(const char* name,const char* path)
	{
		this->path = path;
		this->name = name;

		m_pOldDiffuse = nullptr;
		m_pDiffuseMap = nullptr;
		m_pNormalMap = nullptr;
		m_pHeightMap = nullptr;
		m_pMetallicMap = nullptr;
		m_pRoughnessMap = nullptr;
		m_pAoMap = nullptr;
		m_bloom = false;
		m_generateShadow = true;
		m_useDiffuseTextForOpacity = false;
		m_opacity = 1.0f;
		m_metalness = 0.0f;
		m_roughness = 0.0f;
		m_albedo = Vector3D(255.f / 255.f, 62.f / 255.f, 209.f / 255.f);
		type = ResourceType::MATERIAL;

		if (!std::filesystem::exists(std::filesystem::path(path)))
		{
			std::ofstream{ path };
			SaveInMatFile();
		}

	}

	Material::~Material()
	{

	}

	void Material::Load(const std::string& path)
	{
		loaded = false;
		created = false;

		std::string data;
		Utils::ReadFile(path, data);
		if (!data.empty())
		{
			const int sizeData = 13;
			std::string dataCut[sizeData];
			size_t index = 0;
			size_t baseIndex = 0;
			for (size_t i = 0; i < sizeData; i++)
			{
				index = data.find(',', index + 1);
				dataCut[i] = data.substr(baseIndex, index - baseIndex);
				baseIndex = index + 1;
			}

			for (int i = 0; i < 6; i++)
				m_pathTextures.push_back(dataCut[i]);

			m_bloom = std::stoi(dataCut[6]);
			m_generateShadow = std::stoi(dataCut[7]);
			m_useDiffuseTextForOpacity = std::stoi(dataCut[8]);
			m_opacity = std::stof(dataCut[9]);
			m_metalness = std::stof(dataCut[10]);
			m_roughness = std::stof(dataCut[11]);
			m_albedo = Vector3D::GetFromString(dataCut[12]);
		}

		loaded = true;
		LINFO(path);
	}

	void Material::GetTexturesFromResourcesManager()
	{
		m_pDiffuseMap = SetTexture(m_pathTextures[0], true);
		m_pNormalMap = SetTexture(m_pathTextures[1], false);
		m_pHeightMap = SetTexture(m_pathTextures[2], false);
		m_pMetallicMap = SetTexture(m_pathTextures[3], false);
		m_pRoughnessMap = SetTexture(m_pathTextures[4], false);
		m_pAoMap = SetTexture(m_pathTextures[5], false);
		m_pOldDiffuse = m_pDiffuseMap;
		created = true;
	}

	Texture* Material::SetTexture(std::string path, bool gammaCorrection)
	{
		if (path == " ") return nullptr;

		Texture* texture = nullptr;
		std::string name = Utils::GetNameFromPath(path);

		if (Resources::ResourceManager::HasResource<Texture>(name))
		{
			texture = Resources::ResourceManager::GetResource<Texture>(name);
			texture->SetGammaCorrection(gammaCorrection);
		}
		else
		{
			texture = new Texture(path, Utils::GetNameFromPath(path), gammaCorrection);
			Resources::ResourceManager::AddResource<Texture>(texture);
		}
		
		m_pTextures.push_back(texture);
		return texture;
	}

	bool Material::IsCreated()
	{
		if (created && m_pOldDiffuse == m_pDiffuseMap)
		{
			return true;
		}
		else if (created)
		{
			m_pDiffuseMap->SetGammaCorrection(true);
			m_pOldDiffuse = m_pDiffuseMap;
			return true;
		}
		else if (loaded)
		{
			GetTexturesFromResourcesManager();
			return true;
		}

		return false;
	}

	std::string Material::Save()
	{
		std::string content = "6,";
		content += path + ',';
		content += name + ',';
		return content;
	}

	void Material::SaveInMatFile()
	{
		std::string content;

		content += GetTexturePathToSave(m_pDiffuseMap) + ',';
		content += GetTexturePathToSave(m_pNormalMap) + ',';
		content += GetTexturePathToSave(m_pHeightMap) + ',';
		content += GetTexturePathToSave(m_pMetallicMap) + ',';
		content += GetTexturePathToSave(m_pRoughnessMap) + ',';
		content += GetTexturePathToSave(m_pAoMap) + ',';
		content += std::to_string(m_bloom) + ',';
		content += std::to_string(m_generateShadow) + ',';
		content += std::to_string(m_useDiffuseTextForOpacity) + ',';
		content += std::to_string(m_opacity) + ',';
		content += std::to_string(m_metalness) + ',';
		content += std::to_string(m_roughness) + ',';
		content += m_albedo.ToString() + ',';

		Utils::WriteFile(path, content);
	}

	void Material::DeleteTextures()
	{
		DeleteTexture(m_pDiffuseMap);
		DeleteTexture(m_pNormalMap);
		DeleteTexture(m_pHeightMap);
		DeleteTexture(m_pMetallicMap);
		DeleteTexture(m_pRoughnessMap);
		DeleteTexture(m_pAoMap);
	}

	void Material::DeleteTexture(Texture* text)
	{
		if (text != nullptr)
			delete text;
	}

	std::string Material::GetTexturePathToSave(Texture* texture)
	{
		if (texture != nullptr)
			return texture->path;

		return " ";
	}

	void Material::Unload()
	{

	}
}