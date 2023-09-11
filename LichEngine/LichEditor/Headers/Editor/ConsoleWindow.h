#pragma once
#include <Editor/IEditorWindow.h>

namespace EditorLich
{

	class ConsoleWindow : public IEditorWindow
	{
	public:
		ConsoleWindow();
		~ConsoleWindow();
		void Update() override;

	private:
		bool m_isOpen = false;

	};

}
