#pragma once
#include <unordered_map>

#include <Resources/Shape.h>
#include <Resources/WrapperRapidObj.h>
#undef min
#undef max
#include <ToolBox.h>
#include <Resources/WrapperAssimp.h>
#include <Generated/Model.rfkh.h>


namespace Resources NAMESPACE()
{
	struct LICH_ENGINE BoneInfo
	{
		int id = 0;
		Matrix4D offset;
	};

	class CLASS() LICH_ENGINE Model : public Resource
	{
	public:
		Model();
		Model(const std::string & resourceName, const std::string & resourcePath);
		~Model();

		void Load(const std::string& path) override;
		void Unload() override;
		bool IsCreated() override;
		std::string Save() override;

		inline std::vector<SubModel*>& GetSubModels()						{ return m_pSubModels; }
		inline std::unordered_map<std::string, BoneInfo*>& GetBoneInfoMap() { return m_boneInfoMap; }

	private:
		int m_boneCounter = 0;
		std::vector<SubModel*> m_pSubModels;
		std::unordered_map<std::string, BoneInfo*> m_boneInfoMap;

		//For Obj files (rapidObj)
		void CalculateTangentBitangent(std::vector<Vertex>& triangle);
		void SetVertexObj(Vertex& vertex, ObjResult& result, const ObjIndex index);
		void ProcessShapeObj(std::filesystem::path path, ObjResult& result, const ObjShape& shape, unsigned int& startIndex);
		void LoadMaterialObj(Material* pMaterial, const ObjMaterial objMat, const std::string path);
		void SetVertexInSubModelObj(std::unordered_map<Vertex, uint32_t>& uniqueVertices, std::vector<Vertex> triangle, const int indexShape);
		Texture* LoadMaterialParametersObj(std::string modelPath, std::string text_name, bool gammaCorrection);

		//For other files (assimp)
		void SetVertexBoneDataToDefault(Vertex& vertex);
		void SetVertex(Vertex& vertex, MeshWrap* mesh, const int index);
		void SetVertexBoneData(Vertex& vertex, const int boneID, const float weight);
		void ProcessNode(std::filesystem::path path, NodeWrap* node, const SceneWrap* scene);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, MeshWrap* mesh, const SceneWrap* scene);
		void LoadMaterial(Material* pOurMaterial, const std::string path, MaterialWrap* pMat);
		Texture* LoadMaterialParameters(MaterialWrap* pMat, const std::string path, aiTextureType type, bool gammaCorrection);
		SubModel* ProcessMesh(std::filesystem::path path, MeshWrap* mesh, const SceneWrap* scene);
		Resources_Model_GENERATED
	};
};
File_Model_GENERATED