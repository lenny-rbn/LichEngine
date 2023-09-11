#include <Editor/ConsoleWindow.h>
#include <Debugs/Log.h>

EditorLich::ConsoleWindow::ConsoleWindow()
{
}

EditorLich::ConsoleWindow::~ConsoleWindow()
{
}

void EditorLich::ConsoleWindow::Update()
{
	EditorUI::WindowFlags window_flags = EditorUI::WindowOptions::MenuBar;
	if (EditorUI::CreateWindowUI("Console", &m_isOpen, window_flags))
	{
		if (EditorUI::CreateUI(EditorUI::TypeUI::MenuBar,"Console Menu Bar"))
		{
			if (EditorUI::Button("Clear Console"))
			{
				Debug::Log::ClearLogArray();
			}
			EditorUI::End(EditorUI::TypeUI::MenuBar,__FILE__,__LINE__);
		}


		for (size_t i = 0; i < (Debug::Log::logIndex); i++)
		{
			if (Debug::Log::logArray[i] != "\0") 
			{
				Vector4D color = Vector4D(1, 1, 1, 1);
				if (Debug::Log::logArray[i][1] == 'W')
				{
					color = Vector4D(1, 1, 0, 1);
				}
				if (Debug::Log::logArray[i][1] == 'E')
				{
					color = Vector4D(1, 0, 0, 1);
				}
				EditorUI::WriteColorText(Debug::Log::logArray[i], color);
			}
		}


	}
	EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);

}
