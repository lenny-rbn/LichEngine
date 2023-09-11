#include <pch.h>
#include <Scene\Gizmos.h>


namespace SceneManagement
{
	void Gizmos::SetGO(GameObject* gO)
	{
		if (m_pSelectedGO != nullptr)
			m_pSelectedGO->SetSelectedState(false);

		if (gO == nullptr)
		{
			m_pSelectedGO = nullptr;
			return;
		}

		m_pSelectedGO = gO;
		m_pSelectedGO->SetSelectedState(true);
	}
}