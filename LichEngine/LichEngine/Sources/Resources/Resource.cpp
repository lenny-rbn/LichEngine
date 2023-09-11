#include <pch.h>
#include <Generated/Resource.rfks.h>


namespace Resources
{
	Resource::Resource()
	{
		loaded = false;
		created = false;
		load = std::bind(&Resource::Load, this, std::placeholders::_1);
	}

	Resource::Resource(const std::string& resourceName, const std::string& resourcePath)
	{
		loaded = false;
		created = false;
		name = resourceName;
		path = resourcePath;
		load = std::bind(&Resource::Load, this, std::placeholders::_1);
	}
}