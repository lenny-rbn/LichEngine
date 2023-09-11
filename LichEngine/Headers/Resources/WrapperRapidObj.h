#pragma once
#include <rapidObjLoader/rapidobj.hpp>

#include <Define.h>

typedef rapidobj::Result ObjResult;
typedef rapidobj::Shape ObjShape;
typedef rapidobj::Index ObjIndex;
typedef rapidobj::Material ObjMaterial;

namespace Resources
{
	ObjResult ParseObj(std::filesystem::path filepath);
}