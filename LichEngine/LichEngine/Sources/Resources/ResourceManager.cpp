#include <pch.h>
#include <Resources/Model.h>
#include <UI/Image.h>
#include <Resources/ResourceManager.h>
#include <Camera/Skybox.h>
#include <Resources/WrapperAssimp.h>
#include <Toolbox.h>
#include <Debugs/Log.h>


namespace Resources
{
	ThreadPool* ResourceManager::m_pThreadpool = new ThreadPool();
	std::unordered_map<std::string,Resource*> ResourceManager::m_resources = std::unordered_map<std::string,Resource*>();
	std::vector<std::string> ResourceManager::importedAnimsSkeletons = std::vector<std::string>();

	ResourceManager::~ResourceManager()
	{
		SaveAnimAndSkeletonFiles("SaveAnimSkelFiles.txt");
	}

	void ResourceManager::CreateSpecificResource(ResourceType type,const char* path,const char* name,Resources::ResourceLayer layer)
	{
		switch (type)
		{
		case MODEL:
			CreateResource<Model>(path, name);
			break;
		case SHADER:
			CreateResourceMonothread<Shader>(path, name);
			break;
		case TEXTURE:
			CreateResource<Texture>(path, name);
			break;
		case SKELETON:
			CreateResource<AnimationSystem::Skeleton>(path, name);
			break;
		case SOUND:
			CreateResourceMonothread<SoundInstance>(path, name);
			break;
		case ANIMATION:
			CreateResource<AnimationSystem::Animation>(path, name);
			break;
		case SKYBOX:
			CreateResource<CameraRender::SkyBox>(path, name);
			break;
		case FONT:
			CreateResourceMonothread<UI::Font>(path, name);
			break;
		case MATERIAL:
			CreateResource<Resources::Material>(path, name);
			break;
		default:
			break;
		}

		GetSpecificResource(type,name)->layer = layer;
	}

	Resource* ResourceManager::GetSpecificResource(ResourceType type, const char* name)
	{
		Resource* resourceToGive = nullptr;
		switch (type)
		{
		case MODEL:
			resourceToGive = GetResource<Model>(name);
			break;
		case SHADER:
			resourceToGive = GetResource<Shader>(name);
			break;
		case TEXTURE:
			resourceToGive = GetResource<Texture>(name);
			break;
		case SKELETON:
			resourceToGive = GetResource<AnimationSystem::Skeleton>(name);
			break;
		case SOUND:
			resourceToGive = GetResource<SoundInstance>(name);
			break;
		case ANIMATION:
			resourceToGive = GetResource<AnimationSystem::Animation>(name);
			break;
		case SKYBOX:
			resourceToGive = GetResource<CameraRender::SkyBox>(name);
			break;
		case MATERIAL:
			resourceToGive = GetResource<Resources::Material>(name);
			break;
		case FONT:
			resourceToGive = GetResource<UI::Font>(name);
			break;
		default:
			break;
		}
		return resourceToGive;
	}

	void ResourceManager::Clear()
	{
		for (std::unordered_map<std::string, Resource*>::iterator index = m_resources.begin(); index != m_resources.end(); index++)
		{
			index->second->Unload();
			delete index->second;
			index->second = nullptr;
		}

		m_resources.clear();
	}

	void ResourceManager::ClearScene()
	{
		std::vector<std::string> m_names;
		for (std::unordered_map<std::string, Resource*>::iterator index = m_resources.begin(); index != m_resources.end(); index++)
		{
			if (index->second->type == Resources::ResourceType::SHADER || index->first == "default" || index->first == "folder") continue;

			m_names.push_back(index->first);
		}

		for (size_t i = 0; i < m_names.size(); i++)
		{
			UnloadResource(m_names[i]);
		}
	}

	void ResourceManager::ClearSceneByLayer(ResourceLayer layer)
	{
		std::vector<std::string> m_names;
		for (std::unordered_map<std::string, Resource*>::iterator index = m_resources.begin(); index != m_resources.end(); index++)
		{
			if (index->second->layer != layer) continue;

			m_names.push_back(index->first);
		}

			for (size_t i = 0; i < m_names.size(); i++)
		{
			UnloadResource(m_names[i]);
		}
	}

	void Resources::Create(ResourceType type, const std::string& path, const std::string& name,Resources::ResourceLayer layer)
	{
		ResourceManager::CreateSpecificResource(type, path.c_str(), name.c_str(), layer);
	}

	void ResourceManager::ImportAnimAndSkeletonFiles(const char* path)
	{
		if (std::find(ResourceManager::importedAnimsSkeletons.begin(), ResourceManager::importedAnimsSkeletons.end(), path) != ResourceManager::importedAnimsSkeletons.end())
			return;

		const SceneWrap* scene = LoadScene(importer, path, 0);

		if (scene == nullptr)
			return;

		bool hasBones = false;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			if (!hasBones && scene->mMeshes[i]->HasBones())
				hasBones = true;
		}

		if (hasBones)
		{
			std::string skeletonPath = Utils::remove_extension(path) + "_skeleton.lskel";
			if (!std::filesystem::exists(std::filesystem::path(skeletonPath)))
				std::ofstream{ skeletonPath };
			Utils::WriteFile(skeletonPath, path);
		}

		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			std::string animPath = Utils::remove_extension(path) + "_anim" + std::to_string(i) + ".lanim";
			if (!std::filesystem::exists(std::filesystem::path(animPath)))
				std::ofstream{ animPath };

			std::string animData = (std::string)path + "," + std::to_string(i);
			Utils::WriteFile(animPath, animData);
		}

		ResourceManager::importedAnimsSkeletons.push_back(path);
	}

	void ResourceManager::InitAnimAndSkeletonFiles(const char* path)
	{
		if (!std::filesystem::exists(std::filesystem::path(path)))
			std::ofstream{ path };

		std::string line;
		std::ifstream myfile;
		myfile.open(path);
		if (myfile.is_open())
		{
			while (std::getline(myfile, line))
				importedAnimsSkeletons.push_back(line);

			myfile.close();
		}
		else 
		{
			LWARNING("Can't open file " + (std::string)path);
		}
	}

	void ResourceManager::SaveAnimAndSkeletonFiles(const char* path)
	{
		std::string data;

		for (int i = 0; i < importedAnimsSkeletons.size(); i++)
			data += importedAnimsSkeletons[i] + "\n";

		Utils::WriteFile(path, data);
	}
}