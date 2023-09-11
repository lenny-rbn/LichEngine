#pragma once
#include <Editor/IEditorWindow.h>

namespace EditorLich
{
	class EditorResources
	{
	public:
		EditorResources();
		~EditorResources();

		void LoadFont();
		void ApplyEditorGraphicStyle();
		void InitGraphicsResource();
	private:
		void LoadGrahicsResources();
		const char* m_editorResoucePath =".\\EditorResources";
		const char* m_editorResouceFomtPath =".\\EditorResources\\Fonts";
	};


}