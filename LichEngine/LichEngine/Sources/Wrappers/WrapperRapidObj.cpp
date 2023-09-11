#include <pch.h>
#include <Debugs/Log.h>
#include <Resources/WrapperRapidObj.h>


namespace Resources
{
	void ReportError(const rapidobj::Error& error)
	{
		LERROR(error.code.message());
		if (!error.line.empty())
			LERROR("On line " + std::to_string(error.line_num) + ": \"" + error.line + "\"\n");
	}

	ObjResult ParseObj(std::filesystem::path filepath)
	{
		auto result = rapidobj::ParseFile(filepath,rapidobj::MaterialLibrary::Default());
		if (result.error)
		{
			ReportError(result.error);
			return result;
		}

		rapidobj::Triangulate(result);

		if (result.error)
		{
			ReportError(result.error);
			return result;
		}

		return result;
	}
}