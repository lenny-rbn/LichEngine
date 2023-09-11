#pragma once
#include <Resources/Material.h>
#include <Generated/Shape.rfkh.h>


namespace Resources NAMESPACE()
{
	struct STRUCT() LICH_ENGINE SubModel
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		FIELD() Material* pMaterial = nullptr;

		uint VAO = 0;
		uint VBO = 0;
		uint EBO = 0;

		Resources_SubModel_GENERATED
	};
};
File_Shape_GENERATED