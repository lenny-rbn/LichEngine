#include <pch.h>
#include <Generated/Component.rfks.h>

void SceneManagement::Component::ChangeEnableState()
{
	if (CheckEnableState()) return; 

	p_enableTransition = true;
	m_prevEnableState = isEnable;
}