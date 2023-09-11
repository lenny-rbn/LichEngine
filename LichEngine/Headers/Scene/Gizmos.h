#pragma once
#include <Scene/GameObject.h>

namespace SceneManagement
{
	enum GizmosModificationMode
	{
		POSITION,
		ROTATION,
		SCALE,
	};

	class LICH_ENGINE Gizmos
	{
	private:
		GameObject* m_pSelectedGO = nullptr;
		GizmosModificationMode m_mode = GizmosModificationMode::POSITION;

	public:
		void SetGO(GameObject* gO);
		GameObject* GetGO()										{ return m_pSelectedGO; }
		bool IsActivated()										{ return m_pSelectedGO != nullptr; }

		GizmosModificationMode GetModificationMode()			{ return m_mode; }
		void SetModificationMode(GizmosModificationMode mode)	{ m_mode = mode; }


	};
}