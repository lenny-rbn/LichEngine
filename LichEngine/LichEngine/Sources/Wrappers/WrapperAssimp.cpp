#include <pch.h>
#include <Resources/WrapperAssimp.h>


namespace Resources
{
	const SceneWrap* LoadScene(ImporterWrap& importer, std::string path, unsigned int flags)
	{
		const SceneWrap* scene = importer.ReadFile(path, flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return nullptr;
		}

		return scene;
	}

	Matrix4D ConvertMatrix4x4WrapToMatrix4D(Matrix4x4Wrap matrix)
	{
		Matrix4D ourMatrix;
		ourMatrix.coord[0][0] = matrix.a1; ourMatrix.coord[1][0] = matrix.a2; ourMatrix.coord[2][0] = matrix.a3; ourMatrix.coord[3][0] = matrix.a4;
		ourMatrix.coord[0][1] = matrix.b1; ourMatrix.coord[1][1] = matrix.b2; ourMatrix.coord[2][1] = matrix.b3; ourMatrix.coord[3][1] = matrix.b4;
		ourMatrix.coord[0][2] = matrix.c1; ourMatrix.coord[1][2] = matrix.c2; ourMatrix.coord[2][2] = matrix.c3; ourMatrix.coord[3][2] = matrix.c4;
		ourMatrix.coord[0][3] = matrix.d1; ourMatrix.coord[1][3] = matrix.d2; ourMatrix.coord[2][3] = matrix.d3; ourMatrix.coord[3][3] = matrix.d4;
		return ourMatrix;
	}
}