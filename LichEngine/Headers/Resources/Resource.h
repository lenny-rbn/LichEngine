#pragma once
#include <iostream>
#include <functional>

#include <Define.h>

#include <Generated/Resource.rfkh.h>


namespace Resources NAMESPACE()
{
	enum ResourceType
	{
		MODEL = 0,
		SHADER = 1,
		TEXTURE = 2,
		SKELETON = 3,
		SOUND = 4,
		ANIMATION = 5,
		MATERIAL = 6,
		SKYBOX = 7,
		FONT = 8,
		SCRIPTING = 9,
		NONE = 10,
	};

	enum ResourceLayer
	{
		SCENE = 0,
		MAIN = 1,
		EDITOR = 2,
		NOLAYER = 3,
	};

	class CLASS() LICH_ENGINE Resource
	{
	public:
		bool loaded;
		bool created;

		std::string name;
		std::string path;
		std::function<void(std::string)> load;

		Resource();
		Resource(const std::string& resourceName, const std::string& resourcePath);
		ResourceType type = NONE;
		ResourceLayer layer = SCENE;
		

		virtual void Load(const std::string & path) = 0;
		virtual std::string Save() = 0;
		virtual bool IsCreated() = 0;
		virtual void Unload() = 0;

		Resources_Resource_GENERATED
	};
};
File_Resource_GENERATED