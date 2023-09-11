#pragma once
#include <Resources/Scripting.h>
#include <ToolBox.h>
#include <Generated/MovingCrate.rfkh.h>


class CLASS() LICH_ENGINE MovingCrate : public Resources::Scripting
{
public:
	FIELD() VectorContainer<Vector3D> points;
	FIELD() float speed = 1.f;

	Physics::Transform* transform;
	int targetId = 0;

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	MovingCrate_GENERATED
};
File_MovingCrate_GENERATED