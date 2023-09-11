#pragma once
#include <fstream>
#include <cassert>
#include <filesystem>

#include <Core/RHI.h>


namespace Resources
{
	class Shader : public Resource
	{
	public:
		Shader(const std::string& resourceName, const std::string& resourcePath);
		~Shader();

		void Load(const std::string& path) override;
		void Unload() override;
		bool IsCreated() override { return true; }
		std::string Save() override;

		inline int& GetShaderProgram()										{ return m_shaderProgram; }
		inline std::vector<std::string> GetDatas()							{ return m_datas; }
		inline std::vector<ShaderType> GetShaderTypes()						{ return m_types; }
		inline std::unordered_map<std::string, uint>& GetUniformLocations() { return m_uniformLocations; }

	private:
		int m_shaderProgram;
		std::vector<std::string> m_datas;
		std::vector<ShaderType> m_types;
		std::unordered_map<std::string, uint> m_uniformLocations;
	};
}