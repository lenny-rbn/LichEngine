#include <pch.h>
#include <Debugs/Log.h>
#include <Generated/ToolBox.rfks.h>


bool TestField(const rfk::Field&, void*)
{
	return true;
}

/// <summary>
/// Function to read .txt file
/// </summary>
/// <param name="path"></param>
/// <param name="data">Container of the data of the file read</param>
bool Utils::ReadFile(const std::string& path, std::string& data)
{
	std::string line;
	std::ifstream myfile;
	myfile.open(path);
	if (myfile.is_open())
	{
		LINFO("Open the file : " + path);
		while (std::getline(myfile, line))
		{
			data += line;
			data.push_back('\n');
		}

		myfile.close();
		LINFO("Close the file : " + path);
		return true;
	}
	else
	{
		LERROR("Can't open this file at " + path);
		return false;
	}
}

/// <summary>
/// Write Data into a .txt file
/// </summary>
/// <param name="name"></param>
/// <param name="data"></param>
void Utils::WriteFile(const std::string& name, const std::string& data)
{
	std::ofstream myfile;
	myfile.open(name);
	myfile << data;
	myfile.close();
}

std::string Utils::remove_extension(const std::string& filename)
{
	size_t extension = filename.find_last_of(".");
	if (extension == std::string::npos)
		return filename;

	return filename.substr(0, extension);
}

LAPI std::string Utils::GetNameFromPath(const std::string& path)
{
	std::filesystem::path filepath(path);
	return remove_extension(filepath.filename().string());
}

char* Utils::remove_extensionChr(char* filename)
{
	std::string name = filename;
	size_t extension = name.find_last_of(".");

	if (extension == std::string::npos)
		return filename;

	name = name.substr(0, extension);
	strcpy_s(filename, name.size() + 1, name.c_str());
	return filename;
}


rfk::Vector<const rfk::Field*> SubShapeContainer::GetIndexField(int index)
{
	Resources::SubModel* instance = (*this)[index];
	rfk::Predicate<rfk::Field> test = TestField;
	rfk::Vector<const rfk::Field*> vec = Resources::SubModel::staticGetArchetype().getFieldsByPredicate(test, instance, true, true);
	return vec;
}

Resources::SubModel* SubShapeContainer::GetDataIndexPointer(int index)
{
	return(*this)[index];
}