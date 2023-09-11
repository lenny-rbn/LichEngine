#include <pch.h>
#include <Debugs/Log.h>
#include <Resources/ResourceManager.h>
#include <Animation/Skeleton.h>
#include <Animation/Animation.h>
#include <chrono>
#include <Toolbox.h>
#include <Generated/Model.rfks.h>

using namespace Resources;

Model::Model()
{
	name = "None";
	path = "";
	type = Resources::ResourceType::MODEL;
	m_boneCounter = 0;
}

Model::Model(const std::string& resourceName, const std::string& resourcePath)
{
	name = resourceName;
	path = resourcePath;
	m_boneCounter = 0;
	type = Resources::ResourceType::MODEL;
}

Model::~Model() {}

void Model::Load(const std::string& path)
{
	std::filesystem::path filepath = path;

	if (filepath.extension().string() == ".obj") //Rapid Obj
	{
		ObjResult result = ParseObj(filepath);
		unsigned int baseIndex = m_pSubModels.size();
		for (int i = 0; i < result.shapes.size(); i++)
		{
			ProcessShapeObj(path, result, result.shapes[i], baseIndex);
			baseIndex = m_pSubModels.size();
		}
	}
	else //Assimp	
	{
		ImporterWrap importer;
		const SceneWrap* scene = LoadScene(importer, path, ImportFlags::TRIANGULATE | ImportFlags::FLIPUVS | ImportFlags::CALCTANGENTSPACE | ImportFlags::LIMITBONEWEIGHTS);
		if(scene != nullptr)
			ProcessNode(path, scene->mRootNode, scene);
	}

	loaded = true;
	LINFO(path);
}

/// OBJ ///

void Model::ProcessShapeObj(std::filesystem::path path, ObjResult& result, const ObjShape& shape, unsigned int& startIndex)
{
	int faceCount = 0;
	std::vector<Vertex> face;
	std::vector<int> idMaterials;
	std::vector<std::unordered_map<Vertex, uint32_t>> uniqueVertices{};

	bool firstMat = false;
	int32_t indexMat = -1;
	int firstIndexSubModel = startIndex;
	int count = 0;
	for (const auto& index : shape.mesh.indices)
	{
		Vertex vertex{};
		SetVertexObj(vertex, result, index);
		face.push_back(vertex);
		count++;
		if (face.size() == 3)
		{
			int indexSubModel = firstIndexSubModel;
			std::vector<int>::iterator idMaterialFace = std::find(idMaterials.begin(), idMaterials.end(), shape.mesh.material_ids[faceCount]);
			if (!firstMat || indexMat != -1)
			{
				if (idMaterialFace != idMaterials.end())  //If the material of the face already exists, find the right SubModel
				{
					indexSubModel = startIndex + (int)std::distance(idMaterials.begin(), idMaterialFace);
				}
				else //If it's a new material, create a new SubModel
				{
					SubModel* pNewSubModel = new SubModel();
					indexSubModel = startIndex+ (int)idMaterials.size();
					pNewSubModel->name = shape.name + std::to_string(indexSubModel);
					if (!firstMat)
					{
						firstMat = true;
						firstIndexSubModel =  indexSubModel;
					}

					indexMat = shape.mesh.material_ids[faceCount];
					if (indexMat != -1)
					{
						ObjMaterial objMaterial = result.materials[indexMat];
						idMaterials.push_back(indexMat);
						std::string matName = name + "_" + objMaterial.name;
						std::string matPath = "Resources/Materials/" + matName + ".lmat";
						std::filesystem::path filepath(matPath);

						if (!std::filesystem::exists(filepath))
						{
							Material* newMaterial = new Material(matName.c_str(), matPath.c_str());
							LoadMaterialObj(newMaterial, objMaterial, path.remove_filename().string());
							newMaterial->SaveInMatFile();
							newMaterial->DeleteTextures();
							delete newMaterial;
						}
					}

					m_pSubModels.push_back(pNewSubModel);
					uniqueVertices.push_back(std::unordered_map<Vertex, uint32_t>());
				}
			}
	

			CalculateTangentBitangent(face);
			SetVertexInSubModelObj(uniqueVertices[indexSubModel - startIndex], face, indexSubModel);
			face.clear();
			faceCount++;	
		}
	}
}

void Model::SetVertexObj(Vertex& vertex, ObjResult& result, const ObjIndex index)
{
	vertex.position =
	{
		result.attributes.positions[3 * index.position_index + 0],
		result.attributes.positions[3 * index.position_index + 1],
		result.attributes.positions[3 * index.position_index + 2]
	};

	vertex.uv =
	{
		result.attributes.texcoords[2 * index.texcoord_index + 0],
		1.f - result.attributes.texcoords[2 * index.texcoord_index + 1],
	};

	vertex.normal =
	{
		result.attributes.normals[3 * index.normal_index + 0],
		result.attributes.normals[3 * index.normal_index + 1],
		result.attributes.normals[3 * index.normal_index + 2]
	};
}

void Model::SetVertexInSubModelObj(std::unordered_map<Vertex, uint32_t>& uniqueVertices, std::vector<Vertex> triangle, const int indexShape)
{
	for (int i = 0; i < 3; i++)
	{
		if (uniqueVertices.count(triangle[i]) == 0)
		{
			uniqueVertices[triangle[i]] = static_cast<uint32_t>(m_pSubModels[indexShape]->vertices.size());
			m_pSubModels[indexShape]->vertices.push_back(triangle[i]);
		}
		else
		{
			m_pSubModels[indexShape]->vertices[uniqueVertices[triangle[i]]].tangent += triangle[i].tangent;
			m_pSubModels[indexShape]->vertices[uniqueVertices[triangle[i]]].tangent.Normalize();

			m_pSubModels[indexShape]->vertices[uniqueVertices[triangle[i]]].bitangent += triangle[i].bitangent;
			m_pSubModels[indexShape]->vertices[uniqueVertices[triangle[i]]].bitangent.Normalize();
		}

		m_pSubModels[indexShape]->indices.push_back(uniqueVertices[triangle[i]]);
	}
}

void Model::LoadMaterialObj(Material* pMaterial, const ObjMaterial objMat, const std::string path)
{
	Texture* diffuse = LoadMaterialParametersObj(path, objMat.diffuse_texname, true);
	Texture* normal = LoadMaterialParametersObj(path, objMat.normal_texname, false);
	Texture* height = LoadMaterialParametersObj(path, objMat.displacement_texname, false);
	Texture* metallic = LoadMaterialParametersObj(path, objMat.metallic_texname, false);
	Texture* roughness = LoadMaterialParametersObj(path, objMat.roughness_texname, false);
	Texture* ao = LoadMaterialParametersObj(path, objMat.ambient_texname, false);

	if (diffuse != nullptr)
		pMaterial->SetDiffuse(diffuse);
	if (normal != nullptr)
		pMaterial->SetNormal(normal);
	if (height != nullptr)
		pMaterial->SetHeight(height);
	if (metallic != nullptr)
		pMaterial->SetMetallic(metallic);
	if (roughness != nullptr)
		pMaterial->SetRouhgness(roughness);
	if (ao != nullptr)
		pMaterial->SetAO(ao);
}

Texture* Model::LoadMaterialParametersObj(std::string modelPath, std::string text_name, bool gammaCorrection)
{
	if (!text_name.empty())
	{
		std::filesystem::path textPath = modelPath + text_name;
		Texture* text = new Texture(textPath.string(), Utils::remove_extension(textPath.filename().string()), gammaCorrection);
		return text;
	}

	return nullptr;
}

void Model::CalculateTangentBitangent(std::vector<Vertex>& triangle)
{
	Vector3D edge1 = triangle[1].position - triangle[0].position;
	Vector3D edge2 = triangle[2].position - triangle[0].position;
	Vector2D deltaUV1 = triangle[1].uv - triangle[0].uv;
	Vector2D deltaUV2 = triangle[2].uv - triangle[0].uv;

	float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Vector3D tangent = { f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
						 f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
						 f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z) };
	tangent.Normalize();

	Vector3D bitangent = { f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
						   f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
						   f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z) };
	bitangent.Normalize();

	for (int i = 0; i < 3; i++)
	{
		triangle[i].tangent = tangent;
		triangle[i].bitangent = bitangent;
	}
}

/// OTHER ///

void Model::ProcessNode(std::filesystem::path path, NodeWrap* node, const SceneWrap* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		m_pSubModels.push_back(ProcessMesh(path, scene->mMeshes[node->mMeshes[i]], scene));

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNode(path, node->mChildren[i], scene);
}

SubModel* Model::ProcessMesh(std::filesystem::path path, MeshWrap* mesh, const SceneWrap* scene)
{
	SubModel* pNewSubModel = new SubModel();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};
		SetVertexBoneDataToDefault(vertex);
		SetVertex(vertex, mesh, i);
		pNewSubModel->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		FaceWrap face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			pNewSubModel->indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		StringWrap nameMat;
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, nameMat);
		if (nameMat.length == 0)
			nameMat = "Material";
		std::string ourMatName = name + "_" + nameMat.data;
		std::string ourMatPath = "Resources/Materials/" + ourMatName + ".lmat";
		std::filesystem::path filepath(ourMatPath);

		if (!std::filesystem::exists(filepath))
		{
			Material* newMaterial = new Material(ourMatName.c_str(), ourMatPath.c_str());
			LoadMaterial(newMaterial, path.remove_filename().string(), scene->mMaterials[mesh->mMaterialIndex]);
			newMaterial->SaveInMatFile();
			newMaterial->DeleteTextures();
			delete newMaterial;
		}
	}

	ExtractBoneWeightForVertices(pNewSubModel->vertices, mesh, scene);

	return pNewSubModel;
}

void Model::SetVertex(Vertex& vertex, MeshWrap* mesh, const int index)
{
	vertex.position =
	{
		mesh->mVertices[index].x,
		mesh->mVertices[index].y,
		mesh->mVertices[index].z,
	};

	vertex.uv =
	{
		mesh->mTextureCoords[0][index].x,
		mesh->mTextureCoords[0][index].y,
	};

	vertex.normal =
	{
		mesh->mNormals[index].x,
		mesh->mNormals[index].y,
		mesh->mNormals[index].z,
	};

	vertex.tangent =
	{
		mesh->mTangents[index].x,
		mesh->mTangents[index].y,
		mesh->mTangents[index].z,
	};

	vertex.bitangent =
	{
		mesh->mBitangents[index].x,
		mesh->mBitangents[index].y,
		mesh->mBitangents[index].z,
	};
}

void Model::LoadMaterial(Material* pOurMaterial, const std::string path, MaterialWrap* pMat)
{
	Texture* diffuse = LoadMaterialParameters(pMat, path, aiTextureType_DIFFUSE, true);
	Texture* normal = LoadMaterialParameters(pMat, path, aiTextureType_NORMALS, false);
	Texture* height = LoadMaterialParameters(pMat, path, aiTextureType_DISPLACEMENT, false);
	Texture* ao = LoadMaterialParameters(pMat, path, aiTextureType_LIGHTMAP, false);

	if (diffuse != nullptr)
		pOurMaterial->SetDiffuse(diffuse);
	if (normal != nullptr)
		pOurMaterial->SetNormal(normal);
	if (height != nullptr)
		pOurMaterial->SetHeight(height);
	if (ao != nullptr)
		pOurMaterial->SetAO(ao);
}

Texture* Model::LoadMaterialParameters(MaterialWrap* pMat, const std::string path, aiTextureType type, bool gammaCorrection)
{
	if (pMat->GetTextureCount(type) != 0)
	{
		aiString str;
		pMat->GetTexture(type, 0, &str);

		if (str.data[0] == '*')
			return nullptr;

		Texture* pNewText = new Texture(path + str.C_Str(), Utils::remove_extension(str.C_Str()), gammaCorrection);
		return pNewText;
	}

	return nullptr;
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneIDs[i] = -1;
		vertex.weights[i] = 0.f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, const int boneID, const float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, MeshWrap* mesh, const SceneWrap* scene)
{
	for (int boneIndex = 0; boneIndex < (int)mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_boneInfoMap.find(boneName) == m_boneInfoMap.end())
		{
			BoneInfo* newBoneInfo = new BoneInfo();
			newBoneInfo->id = m_boneCounter;
			newBoneInfo->offset = ConvertMatrix4x4WrapToMatrix4D(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_boneInfoMap[boneName] = newBoneInfo;
			boneID = m_boneCounter;
			m_boneCounter++;
		}
		else
		{
			boneID = m_boneInfoMap[boneName]->id;
		}
		assert(boneID != -1);
		VertexWeightWrap* weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void Model::Unload()
{
	for (int i = 0; i < m_pSubModels.size(); i++)
		delete m_pSubModels[i];

	for (std::unordered_map<std::string, BoneInfo*>::iterator it = m_boneInfoMap.begin(); it != m_boneInfoMap.end(); it++)
		delete it->second;

	if (created)
		WrapperRHI::RHI::Delete(ResourceGraphicType::MODEL, this);
}

std::string Resources::Model::Save()
{
	std::string content = "0,";
	content += path + ',';
	content += name + ',';
	return content;
}

bool Resources::Model::IsCreated()
{
	if (created)
	{
		return true;
	}
	else if (loaded && !created)
	{
		WrapperRHI::RHI::Create(ResourceGraphicType::MODEL, this);
		created = true;
		return true;
	}

	return false;
}