#include <pch.h>
#include <Debugs/Log.h>
#include <Resources/WrapperSTBI.h>
#include <Generated/Texture.rfks.h>


namespace Resources
{
	Texture::Texture()
	{
		m_width = 0;
		m_height = 0;
		m_texture = 0;
		m_sampler = 0;
		m_nrChannels = 0;
		m_pData = nullptr;
		m_gammaCorrection = false;

		name = "";
		path = "";
		type = TEXTURE;
	}

	Texture::Texture(const std::string& resourcePath, const std::string& resourceName, const bool doGammaCorrection)
	{
		m_width = 0;
		m_height = 0;
		m_texture = 0;
		m_sampler = 0;

		m_nrChannels = 0;
		m_pData = nullptr;
		m_gammaCorrection = doGammaCorrection;

		name = resourceName;
		path = resourcePath;

		type = TEXTURE;
	}

	Texture::Texture(const std::string& resourceName, const std::string& resourcePath)
	{
		m_width = 0;
		m_height = 0;
		m_texture = 0;
		m_sampler = 0;
		m_nrChannels = 0;
		m_pData = nullptr;

		name = resourceName;
		path = resourcePath;
		type = TEXTURE;
	}

	Texture::~Texture() {}

	void Texture::Load(const std::string& path)
	{
		std::filesystem::path filepath(path);
		if (std::filesystem::exists(filepath))
		{
			m_pData = LoadTextureData(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
			loaded = true;
			LINFO(path);
		}
		else
		{
			LWARNING("Texture " + path + " doesn't exist\n");
		}
	}

	std::string Texture::Save()
	{
		std::string content = "2,";
		content += path + ',';
		content += name + ',';
		return content;
	}

	void Texture::Unload()
	{
		if (m_pData != nullptr)
			FreeTextureData(m_pData);

		if (created)
			WrapperRHI::RHI::Delete(ResourceGraphicType::TEXTURE, this);
	}

	bool Texture::IsCreated()
	{
		if (created)
		{
			return true;
		}
		else if (loaded && !created)
		{
			WrapperRHI::RHI::Create(ResourceGraphicType::TEXTURE, this);
			created = true;
			return true;
		}

		return false;
	}
}