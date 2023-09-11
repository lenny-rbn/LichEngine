#pragma once

#include <map>
#include <filesystem>
#include <Resources/ResourceManager.h>
namespace EditorLich
{
	struct AssetData
	{
		std::string name;
		std::filesystem::path path;
		Resources::ResourceType type;
	};

	class AssetResources
	{
	public:
		AssetResources();
		~AssetResources();

		void AddAssetResouce(std::filesystem::path path);
		AssetData* GetAsset(Resources::ResourceType type,std::string name);
		std::vector<AssetData*> GetAssetType(Resources::ResourceType type);
		size_t GetAssetCount() {return m_assetList.size(); }

		bool IsScene(std::filesystem::path path);
		bool IsMaterial(std::filesystem::path path);
		bool isSkybox(std::filesystem::path path);

	private:
		std::multimap<Resources::ResourceType,AssetData*> m_assetList;
		std::map<std::string,AssetData*> m_asseScenetList;

		const char* m_textureExtension[3] = {".jpg",".png", ".jpeg"};
		const char* m_shaderExtension[1] = {".ls" };
		const char* m_modelExtension[3] = {".obj",".fbx", ".dae"};
		const char* m_soundExtension[3] = {".wav",".mp3",".ogg"};
		const char* m_scriptExtension[2] = {".h",".hpp"};
		const char* m_sceneExtension[1] = {".lscene"};
		const char* m_materialExtension[1] = {".lmat"};
		const char* m_animExtension[1] = {".lanim"};
		const char* m_skeletonExtension[1] = { ".lskel"};
		const char* m_skyboxExtension[1] = { ".lskybox" };
		const char* m_fontExtension[2] = { ".ttf",".TTF"};


		void CheckExtensionFile(const char* extensionArray[],const size_t size,Resources::ResourceType type,std::filesystem::path path);
		void CheckSceneExtensionFile(std::filesystem::path path);

	};


}