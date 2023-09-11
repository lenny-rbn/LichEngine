#pragma once

#include <vector>

#include <Editor/EditorResources.h>
#include <Editor/DockingWindow.h>

namespace EditorLich
{
	class Editor
	{
	public:
		Editor();
		~Editor();

		void Run();
	private:

		void Init();
		void Update();
		void Draw(LichWindow* window);
		void Close();

		void InitWindow();
		void InitEditor();
		void GetEngineData();
		void CreateEditorChildrenWindow();

		Core::App* m_pApp;
		LichWindow* m_pWindow;
		EngineData* m_pEngineData;
		EditorResources m_editorResources;
		EditorUI::IOSystem* m_pIO;
		EditorLich::DockingWindow* m_pDockingWindow;
		
	
		
	};


}