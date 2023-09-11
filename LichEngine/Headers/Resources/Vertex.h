#pragma once
#include <Maths.h>

#define MAX_BONE_INFLUENCE 4

struct LICH_ENGINE Vertex
{
	Vector3D position;
	Vector3D normal;
	Vector2D uv;
	Vector3D tangent;
	Vector3D bitangent;

	int boneIDs[MAX_BONE_INFLUENCE] = {};
	float weights[MAX_BONE_INFLUENCE] = {};

	bool operator==(const Vertex& other) const
	{
		return (position == other.position
			&& normal == other.normal
			&& uv == other.uv);
	}
};

namespace std 
{
	template<> struct hash<Vector3D> {
		size_t operator()(Vector3D const& vec) const {
			return ((hash<float>()(vec.x) ^
				(hash<float>()(vec.y) << 1)) >> 1) ^
				(hash<float>()(vec.z) << 1);
		}
	};

	template<> struct hash<Vector2D> 
	{
		size_t operator()(Vector2D const& vec) const 
		{
			return (std::hash<float>{}(vec.x) ^ 
				   (std::hash<float>{}(vec.y) << 1)); 
		}
	};

	//check
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<Vector3D>()(vertex.position) ^
				(hash<Vector3D>()(vertex.normal) << 1)) >> 1) ^
				(hash<Vector2D>()(vertex.uv) << 1);
		}
	};
}