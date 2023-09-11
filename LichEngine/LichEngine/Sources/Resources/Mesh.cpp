#include <pch.h>
#include <Core/RHI.h>
#include <Resources/ResourceManager.h>
#include <Generated/Mesh.rfks.h>

namespace Renderer
{
	Mesh::Mesh(SceneManagement::GameObject* gO)
	{
		p_pGameObject = gO;
		m_isEmpty = true;
		m_pModel = new Resources::Model();
		m_materialSet = false;
		componentType = SceneManagement::ComponentType::MESH;
	}

	Mesh::Mesh(SceneManagement::GameObject* gO, Resources::Model* modelGive)
	{
		p_pGameObject = gO;
		m_pModel = modelGive;
		m_materialSet = false;
		componentType = SceneManagement::ComponentType::MESH;
	}

	Mesh::~Mesh()
	{
		m_pModel = nullptr;
	}

	void Mesh::SetModel(Resources::Model* model)
	{
		if (m_isEmpty)
		{
			delete(m_pModel);
			m_isEmpty = false;
		}
		m_pModel = model;
		m_pMaterial.clear();
		m_materialSet = false;
	}

	bool Mesh::HasMaterialsGeneratingShadow()
	{
		for (std::vector<Resources::Material*>::iterator it = m_pMaterial.begin(); it != m_pMaterial.end(); it++)
		{
			if ((*it) != nullptr && (*it)->IsGeneratingShadow())
				return true;
		}

		return false;
	}

	bool Mesh::HasTransparentMaterials()
	{
		for (std::vector<Resources::Material*>::iterator it = m_pMaterial.begin(); it != m_pMaterial.end(); it++)
		{
			if ((*it) != nullptr && ((*it)->GetOpacity() < 1.0f || (*it)->IsUsingDiffuseTextForOpacity()))
				return true;
		}

		return false;
	}

	void Mesh::Update(Physics::Transform& transform)
	{
		if (m_pModel == nullptr)
		{
			m_pMaterial.resize(0);
			return;
		}

		if (m_pModel->IsCreated() && m_pModel->GetSubModels().size() != m_pMaterial.size())
			m_pMaterial.resize(m_pModel->GetSubModels().size());
	}

	void Mesh::LoadData(std::string data)
	{
		const int sizeData = 4;
		std::string dataCut[sizeData];
		size_t index = 0;
		size_t baseIndex = 0;
		for (size_t i = 0; i < sizeData; i++)
		{
			index = data.find(',', baseIndex + 1);
			dataCut[i] = data.substr(baseIndex, index - baseIndex);
			baseIndex = index + 1;
		}
		m_pModel = Resources::ResourceManager::GetResource<Resources::Model>(dataCut[0]);
		m_pModel->IsCreated();
		m_forwardRendering = std::stoi(dataCut[1]);

		int nbMaterials = std::stoi(dataCut[2]);
		if (nbMaterials != 0)
		{
			std::string materialName;
			size_t indexMat = 0;
			size_t baseMatIndex = 0;
			for (int i = 0; i < nbMaterials; i++)
			{
				indexMat = dataCut[3].find('/', baseMatIndex + 1);
				materialName = dataCut[3].substr(baseMatIndex, indexMat - baseMatIndex);
				baseMatIndex = indexMat + 1;
				if (materialName != "none")
					m_pMaterial.push_back(Resources::ResourceManager::GetResource<Resources::Material>(materialName));
				else
					m_pMaterial.push_back(nullptr);
			}
			m_materialSet = true;
		}
	}

	std::string Mesh::SaveData()
	{
		std::string content = "~4,";
		content += m_pModel->name + ',';
		content += std::to_string(m_forwardRendering) + ',';
		content += std::to_string(m_pMaterial.size()) + ',';

		for (std::vector<Resources::Material*>::iterator index = m_pMaterial.begin(); index != m_pMaterial.end(); index++)
		{
			if((*index) != nullptr)
				content += (*index)->name + '/';
			else
				content += "none" + '/';
		}

		content += ',';

		return content;
	}

	void Mesh::ApplyEditorChange()
	{

	}

	Resources::Material* Mesh::GetMaterial(int index)
	{
		if (index < m_pMaterial.size())
			return m_pMaterial[index];

		return nullptr;
	}
}