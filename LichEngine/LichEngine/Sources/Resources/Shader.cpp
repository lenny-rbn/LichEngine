#include <pch.h>
#include <Debugs/Log.h>
#include <Resources/Shader.h>


namespace Resources
{
	Shader::Shader(const std::string& resourceName, const std::string& resourcePath)
	{
		m_shaderProgram = -1;
		name = resourceName;
		path = resourcePath;
		type = SHADER;
		layer = MAIN;
	}

	Shader::~Shader() {}

	void Shader::Load(const std::string& path)
	{
		std::filesystem::path filepath(path);
		std::ifstream file;
		std::string line;
		std::string data;

		if (std::filesystem::exists(filepath))
		{
			file.open(filepath.c_str(), std::ifstream::in);
			while (std::getline(file, line))
			{
				if (line == "##vert##")
				{
					data.push_back('\0');
					m_datas.push_back(data);
					data.clear();
					m_types.push_back(ShaderType::VERTEXSHADER);
				}
				else if (line == "##frag##")
				{
					data.push_back('\0');
					m_datas.push_back(data);
					data.clear();
					m_types.push_back(ShaderType::FRAGMENTSHADER);
				}
				else if (line == "##geom##")
				{
					data.push_back('\0');
					m_datas.push_back(data);
					data.clear();
					m_types.push_back(ShaderType::GEOMETRYSHADER);
				}
				else
				{
					data.append(line);
				}
				data.push_back('\n');
			}

			data.push_back('\0');
			m_datas.push_back(data);
			m_datas.erase(m_datas.begin());
			file.close();
			WrapperRHI::RHI::Create(ResourceGraphicType::SHADER, this);
			loaded = true;
			LINFO(path);
		}
		else
		{
			LWARNING("Shader" + path + "doesn't exist\n");
		}
	}

	void Shader::Unload()
	{
		WrapperRHI::RHI::Delete(ResourceGraphicType::SHADER, this);
	}

	std::string Shader::Save()
	{
		std::string content = "1,";
		content += path + ',';
		content += name + ',';
		return content;
	}
}