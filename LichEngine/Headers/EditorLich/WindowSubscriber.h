#pragma once
#include <Maths.h>


namespace Core::Editor
{
	class WindowSubscriber
	{
	public:
		WindowSubscriber() {};
		~WindowSubscriber() {};

		virtual  void SetWindowSize(Vector2D size) = 0;

	protected:
		bool m_isChange = false;
	};
}