#pragma once
#include <unordered_map>

#include <Resources/Shader.h>
#include <Resources/ThreadPool.h>
#include <Animation/Skeleton.h>
#include <Animation/Animation.h>
#include <Resources/SoundInstance.h>
#include <UI/Font.h>


namespace Resources
{

	LAPI void Create(ResourceType type, const std::string& path,const std::string& name,Resources::ResourceLayer layer = Resources::ResourceLayer::SCENE);
	
	class LICH_ENGINE ResourceManager
	{
	private:
		static ThreadPool* m_pThreadpool;
		static std::unordered_map<std::string, Resource*> m_resources;
		static std::vector<std::string> importedAnimsSkeletons;
		ImporterWrap importer;

	public:

		~ResourceManager();

		template <typename T>
		static void CreateResource(const char* path,const char* name)
		{
			if (HasResource<T>(name))
				return;

			T* resource = new T(name, path);
			std::filesystem::path filePath(path);
			std::string extension = filePath.extension().string();

			if (extension == ".dae" || extension == ".fbx" || extension == ".lskel" || extension == ".lanim")
				m_pThreadpool->AddTasksAssimp(Task(resource->load, path));
			else
				m_pThreadpool->AddTasks(Task(resource->load, path));

			m_resources.insert(std::pair<std::string, Resource*>(name, (Resource*)resource));
		}

		template <typename T>
		static void CreateResourceMonothread(const char* path, const char* name)
		{
			if (HasResource<T>(name))
				return;

			T* resource = new T(name,path);
			resource->Load(path);

			m_resources.insert(std::pair<std::string, Resource*>(name, (Resource*)resource));
		}

		template <typename T>
		static void AddResource(T* resource)
		{
			Resource* res = (Resource*)resource;
			if (HasResource<T>(res->name))
				return;

			std::filesystem::path filePath(res->path);
			std::string extension = filePath.extension().string();

			if (extension == ".dae" || extension == ".fbx" || extension == ".lskel" || extension == ".lanim")
				m_pThreadpool->AddTasksAssimp(Task(resource->load, res->path));
			else
				m_pThreadpool->AddTasks(Task(resource->load, res->path));

			m_resources.insert(std::pair<std::string, Resource*>(res->name, (Resource*)resource));
		}

		template <typename T>
		static bool HasResource(const std::string& name)
		{
			auto itr = m_resources.find(name);

			if (itr != m_resources.end()/* && itr->second->loaded*/)
				return true;
			else
				return false;

		}

		template <typename T>
		static T* GetResource(const std::string& name)
		{
			auto itr = m_resources.find(name);

			if (itr != m_resources.end())
				return dynamic_cast<T*>(itr->second);
			else
				return nullptr;

		}

		static void UnloadResource(const std::string& name)
		{
			auto itr = m_resources.find(name);

			if (itr != m_resources.end())
			{
				itr->second->Unload();
				delete itr->second;
				itr->second = nullptr;
				m_resources.erase(itr);
			}
		}

		static void Clear();
		static void ClearScene();
		static void ClearSceneByLayer(ResourceLayer layer);
		static void CreateSpecificResource(ResourceType type,const char* path,const char* name,Resources::ResourceLayer layer = Resources::ResourceLayer::SCENE);
		static Resource* GetSpecificResource(ResourceType type,const char* name);

		void ImportAnimAndSkeletonFiles(const char* path);
		static void InitAnimAndSkeletonFiles(const char* path);
		static void SaveAnimAndSkeletonFiles(const char* path);


		static ThreadPool* GetThreadPool() { return m_pThreadpool; }
		static bool LoadFinish() { return m_pThreadpool->IsAllTaskDone(); }
	};
}