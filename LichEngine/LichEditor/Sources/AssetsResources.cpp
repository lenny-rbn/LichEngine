#include <Editor/AssetsResources.h>

EditorLich::AssetResources::AssetResources()
{
}

EditorLich::AssetResources::~AssetResources()
{
	for (auto p : m_assetList)
	{
		delete(p.second);
	}
	m_assetList.clear();
}

void EditorLich::AssetResources::AddAssetResouce(std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();

	CheckExtensionFile(m_shaderExtension, IM_ARRAYSIZE(m_shaderExtension), Resources::ResourceType::SHADER, path);
	CheckExtensionFile(m_textureExtension, IM_ARRAYSIZE(m_textureExtension), Resources::ResourceType::TEXTURE, path);
	CheckExtensionFile(m_modelExtension, IM_ARRAYSIZE(m_modelExtension), Resources::ResourceType::MODEL, path);
	CheckExtensionFile(m_soundExtension, IM_ARRAYSIZE(m_soundExtension), Resources::ResourceType::SOUND, path);
	CheckExtensionFile(m_scriptExtension, IM_ARRAYSIZE(m_scriptExtension), Resources::ResourceType::SCRIPTING, path);
	CheckExtensionFile(m_materialExtension, IM_ARRAYSIZE(m_materialExtension), Resources::ResourceType::MATERIAL, path);
	CheckExtensionFile(m_skyboxExtension, IM_ARRAYSIZE(m_skyboxExtension), Resources::ResourceType::SKYBOX, path);
	CheckExtensionFile(m_animExtension, IM_ARRAYSIZE(m_animExtension), Resources::ResourceType::ANIMATION, path);
	CheckExtensionFile(m_skeletonExtension, IM_ARRAYSIZE(m_skeletonExtension), Resources::ResourceType::SKELETON, path);
	CheckExtensionFile(m_fontExtension, IM_ARRAYSIZE(m_fontExtension), Resources::ResourceType::FONT, path);
	CheckSceneExtensionFile(path);
}

EditorLich::AssetData* EditorLich::AssetResources::GetAsset(Resources::ResourceType type, std::string name)
{
	auto test = m_assetList.equal_range(type);
	for (auto i = test.first; i != test.second; i++)
	{
		if (i->second->name == name)
		{
			return i->second;
		}
	}

	return nullptr;
}

std::vector<EditorLich::AssetData*> EditorLich::AssetResources::GetAssetType(Resources::ResourceType type)
{
	std::vector<AssetData*> data;
	auto test = m_assetList.equal_range(type);
	for (auto i = test.first; i != test.second; i++)
	{
		data.push_back(i->second);
	}
	return data;
}

bool EditorLich::AssetResources::IsScene(std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();
	for (size_t i = 0; i < 1; i++)
	{
		if (extensionFile == m_sceneExtension[i])
		{
			return true;
		}
	}
	return false;
}

bool EditorLich::AssetResources::IsMaterial(std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();
	for (size_t i = 0; i < 1; i++)
	{
		if (strcmp(extensionFile.c_str(), m_materialExtension[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

bool EditorLich::AssetResources::isSkybox(std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();
	for (size_t i = 0; i < 1; i++)
	{
		if (strcmp(extensionFile.c_str() , m_skyboxExtension[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

void EditorLich::AssetResources::CheckExtensionFile(const char* extensionArray[], const size_t size, Resources::ResourceType type, std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();
	if (GetAsset(type, path.filename().string())) return;

	for (size_t i = 0; i < size; i++)
	{
		if (extensionFile == extensionArray[i])
		{
			
			AssetData* data = new AssetData();
			data->name = path.filename().string();
			data->path = path.string();
			data->type = type;
			m_assetList.insert(std::pair< Resources::ResourceType, AssetData*>(type, data));
			break;
		}
	}
}

void EditorLich::AssetResources::CheckSceneExtensionFile(std::filesystem::path path)
{
	const std::string extensionFile = path.extension().string();
	for (size_t i = 0; i < 1; i++)
	{
		if (extensionFile == m_sceneExtension[i])
		{
			AssetData* data = new AssetData();
			data->name = path.filename().string();
			data->path = path.string();
			m_asseScenetList.insert(std::pair< std::string, AssetData*>(data->name, data));
			break;
		}
	}
}
