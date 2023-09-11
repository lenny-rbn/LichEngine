#include <pch.h>
#include <iostream>
#include <filesystem>
#include <sstream>

#include <Camera/Skybox.h>
#include <Core/RHI.h>
#include <Resources/ResourceManager.h>
#include <Debugs/Log.h>
#include <Toolbox.h>
#include <Generated/Skybox.rfks.h>

namespace CameraRender
{
	SkyBox::SkyBox(const char* name, const char* path)
	{
		this->name = name;
		this->path = path;
		textureSet = false;
		type = Resources::SKYBOX;

		for (int i = 0; i < 6; i++)
			m_faces.push_back(nullptr);

		m_model = Resources::ResourceManager::GetResource<Resources::Model>("CubeSky");
		if (!std::filesystem::exists(path))
		{
			SaveInSkyboxFile();
		}
	}

	SkyBox::~SkyBox()
	{

	}

	void SkyBox::Create()
	{
		WrapperRHI::RHI::CreateCubeMap(m_faces, m_cubemap);
	}

	void SkyBox::UpdateCubemap()
	{
		for (int i = 0; i < 6; i++)
		{
			Resources::Texture* face = GetTextureInSkybox(SkyboxFace(i));
			if (m_faces[i] != face && face->loaded)
			{
				WrapperRHI::RHI::UpdateCubemapFace(SkyboxFace(i), face, m_cubemap);
				m_faces[i] = face;
			}
		}
	}

	void SkyBox::Load(const std::string& path)
	{
		loaded = false;
		created = false;
		textureSet = false;

		std::string content;
		Utils::ReadFile(path, content);

		std::string line;
		std::istringstream iss(content);

		for (int i = 0; i < 6; i++)
		{
			std::getline(iss, line);
			std::filesystem::path facepath(line);
			m_pathFaces.push_back(facepath);
		}

		loaded = true;
	}

	void SkyBox::GetResources()
	{
		for (int i = 0; i < 6; i++)
		{
			std::string name = Utils::remove_extension(m_pathFaces[i].filename().string());
			std::string path = m_pathFaces[i].string();

			if (!Resources::ResourceManager::HasResource<Resources::Texture>(name))
				Resources::ResourceManager::CreateResource<Resources::Texture>(path.c_str(), name.c_str());

			Resources::Texture* face = Resources::ResourceManager::GetResource<Resources::Texture>(name);
			SetTextureInSkybox(face, SkyboxFace(i));
			m_faces[i] = (Resources::Resource*)face;
		}

		textureSet = true;
	}

	std::string SkyBox::Save()
	{
		std::string content = "7,";
		content += path + ',';
		content += name + ',';
		return content;
	}

	void SkyBox::SaveInSkyboxFile()
	{
		std::string data;
		if (m_right != nullptr)
			data += m_right->path + "\n";
		else
			data += "none \n";

		if (m_left != nullptr)
			data += m_left->path + "\n";
		else
			data += "none \n";

		if (m_top != nullptr)
			data += m_top->path + "\n";
		else
			data += "none \n";
		if (m_bottom != nullptr)
			data += m_bottom->path + "\n";
		else
			data += "none \n";
		if (m_front != nullptr)
			data += m_front->path + "\n";
		else
			data += "none \n";
		if (m_back != nullptr)
			data += m_back->path + "\n";
		else
			data += "none \n";
		Utils::WriteFile(path, data);
	}

	bool SkyBox::IsCreated()
	{
		UpdateCubemap();

		if (created)
		{
			return true;
		}
		else if (loaded && !textureSet)
		{
			GetResources();
		}
		else if (loaded && textureSet)
		{
			for (std::vector<Resources::Resource*>::iterator it = m_faces.begin(); it != m_faces.end(); it++)
			{
				if ((*it) == nullptr || !(*it)->loaded)
					return false;
			}

			if (m_model != nullptr && !m_model->IsCreated())
				return false;

			Create();
			created = true;
		}

		return false;
	}

	void SkyBox::Unload()
	{
		WrapperRHI::RHI::Delete(ResourceGraphicType::SKYBOX, this);
	}

	void SkyBox::SetTextureInSkybox(Resources::Texture* texture, SkyboxFace face)
	{
		switch (face)
		{
		case TOPFACE:
			m_top = texture;
			break;
		case BOTTOMFACE:
			m_bottom = texture;
			break;
		case FRONTFACE:
			m_front = texture;
			break;
		case BACKFACE:
			m_back = texture;
			break;
		case RIGHTFACE:
			m_right = texture;
			break;
		case LEFTFACE:
			m_left = texture;
			break;
		default:
			break;
		}
	}

	Resources::Texture* SkyBox::GetTextureInSkybox(SkyboxFace face)
	{
		switch (face)
		{
		case TOPFACE:
			return m_top;
			break;
		case BOTTOMFACE:
			return m_bottom;
			break;
		case FRONTFACE:
			return m_front;
			break;
		case BACKFACE:
			return m_back;
			break;
		case RIGHTFACE:
			return m_right;
			break;
		case LEFTFACE:
			return m_left;
			break;
		default:
			break;
		}
	}
}

