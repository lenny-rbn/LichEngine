#pragma once
#include <Resources/Scripting.h>
#include <Generated/Look.rfkh.h>


class CLASS() LICH_ENGINE Look : public Resources::Scripting
{
public:
	FIELD() float camSensitivity = 0.f;

	double oldMouseX;
	double oldMouseY;

	Physics::Transform* playerT;

	void Start() override;
	void UpdateScript() override;

	bool Started() override { return started; }

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	Look_GENERATED
};
File_Look_GENERATED