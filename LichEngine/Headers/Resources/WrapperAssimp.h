#pragma once
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Maths.h>


namespace Resources
{
	typedef Assimp::Importer ImporterWrap;
	typedef aiScene SceneWrap;
	typedef aiNode NodeWrap;
	typedef aiMesh MeshWrap;
	typedef aiAnimation AnimationWrap;
	typedef aiNodeAnim NodeAnimWrap;
	typedef aiBone BoneWrap;
	typedef aiMaterial MaterialWrap;
	typedef aiFace FaceWrap;
	typedef aiString StringWrap;
	typedef aiVertexWeight VertexWeightWrap;
	typedef aiMatrix4x4 Matrix4x4Wrap;

	enum ImportFlags
	{
		CALCTANGENTSPACE = aiProcess_CalcTangentSpace,
		JOINIDENTICALVERTICES = aiProcess_JoinIdenticalVertices,
		MAKELEFTHANDED = aiProcess_MakeLeftHanded,
		TRIANGULATE = aiProcess_Triangulate,
		REMOVECOMPONENT = aiProcess_RemoveComponent,
		GENNORMALS = aiProcess_GenNormals,
		GENSMOOTHNORMALS = aiProcess_GenSmoothNormals,
		SPLITLARGEMESHES = aiProcess_SplitLargeMeshes,
		PRETRANSFORMVERTICES = aiProcess_PreTransformVertices,
		LIMITBONEWEIGHTS = aiProcess_LimitBoneWeights,
		VALIDATEDATASTRUCTURE = aiProcess_ValidateDataStructure,
		IMPROVECACHELOCALITY = aiProcess_ImproveCacheLocality,
		REMOVEREDUNDANTMATERIALS = aiProcess_RemoveRedundantMaterials,
		FIXINFACINGNORMALS = aiProcess_FixInfacingNormals,
		SORTBYPTYPE = aiProcess_SortByPType,
		FINDDEGENERATES = aiProcess_FindDegenerates,
		FINDINVALIDDATA = aiProcess_FindInvalidData,
		GENUVCOORDS = aiProcess_GenUVCoords,
		TRANSFORMUVCOORDS = aiProcess_TransformUVCoords,
		FINDINSTANCES = aiProcess_FindInstances,
		OPTIMIZEMESHES = aiProcess_OptimizeMeshes,
		OPTIMIZEGRAPH = aiProcess_OptimizeGraph,
		FLIPUVS = aiProcess_FlipUVs,
		FLIPWINDINGORDER = aiProcess_FlipWindingOrder,
		SPLITBYBONECOUNT = aiProcess_SplitByBoneCount,
		DEBONE = aiProcess_Debone,
	};

	const SceneWrap* LoadScene(ImporterWrap& importer, std::string path, unsigned int flags);
	Matrix4D ConvertMatrix4x4WrapToMatrix4D(Matrix4x4Wrap matrix);
}