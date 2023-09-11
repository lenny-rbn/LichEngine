#pragma once
#include <Maths.h>


struct LICH_ENGINE AppSettings
{
	int width = 0;
	int height = 0;

	std::string name;
	 
	void SetWindowSize(Vector2D size) { width = (int)size.x; height = (int)size.y; }
};