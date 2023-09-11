#pragma once
#include <Core/WrapperWindow.h>


namespace Core
{
	class LICH_ENGINE TimeManager
	{
	public:
		TimeManager();
		~TimeManager();

		void  Update();

		static double GetDeltaTime();
		static double GetFixedUpdate();
		static double GetGeneralTime();

	private:
		static double m_timeGeneral;
		static double m_deltaTime;
		static double m_fixedDeltaTime;

		double m_prevFrameTime;
	};
}