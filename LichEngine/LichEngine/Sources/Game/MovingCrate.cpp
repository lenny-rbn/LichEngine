#include <pch.h>
#include <Core/TimeManager.h>
#include <Generated/MovingCrate.rfks.h>

void MovingCrate::Start()
{
	transform = gameObject->GetTransform();
}

void MovingCrate::UpdateScript()
{
	Vector3D dir = *points[targetId] - transform->GetGlobalPosition();

	if (dir.Magnitude() <= 0.01f)
	{
		targetId++;

		if (targetId > points.size() - 1)
			targetId = 0;
	}

	transform->Translate(Vector3D::Normalize(dir) * speed * Core::TimeManager::GetDeltaTime());
}

std::string  MovingCrate::SaveScript()
{
	std::string content = std::to_string(speed) + ',';
	content += std::to_string(points.size()) + ',';

	for (int i = 0; i < points.size(); i++)
		content += points[i]->ToString() + ',';

	return content;
}

void  MovingCrate::LoadScript(std::string data)
{
	const int sizeData = 3;
	std::string dataCut[sizeData];
	size_t index = 0, baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		dataCut[i] = data.substr(baseIndex, index - baseIndex);
		baseIndex = index + 1;
	}

	speed = std::stof(dataCut[1]);
	const int newSizeData = std::stoi(dataCut[2]);

	for (size_t i = 0; i < newSizeData; i++)
	{
		index = data.find(',', baseIndex + 1);
		Vector3D vector = Vector3D::GetFromString(data.substr(baseIndex, index - baseIndex));
		points.push_back(new Vector3D(vector));
		baseIndex = index + 1;
	}
}