#pragma once
#include <Resources/Scripting.h>
#include <UI/Text.h>
#include <Generated/Throw.rfkh.h>


class CLASS() LICH_ENGINE Throw : public Resources::Scripting
{
public:

	FIELD() float distanceToGrab = 0.f;
	FIELD() float forceThrow = 0.f;
	FIELD() float angleToGrab = 10.f;


	std::vector<Physics::Transform*> tools;
	Physics::Transform* currentTool;
	Physics::Transform* toolsParent;
	UI::Text* pressText;

	void TryGetTool();
	void TryThrowTool();
	void Reload();

	void Start() override;
	void UpdateScript() override;

	std::string SaveScript() override;
	void LoadScript(std::string data) override;

	Throw_GENERATED
};
File_Throw_GENERATED
