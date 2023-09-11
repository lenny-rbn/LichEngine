#pragma once
#include<Editor/WrappeImGUI.h>

namespace EditorLich
{
	class IEditorWindow
	{
	public:
		~IEditorWindow() {};
		virtual void Update() = 0;
		bool IsInWindow(Vector2D pos)
		{
			Vector2D windowPos = EditorUI::GetWindowPos();
			Vector2D windowSize = EditorUI::GetWindowSize();

			if (pos.x > windowPos.x && pos.y > windowPos.y && pos.x < windowPos.x + windowSize.x && pos.y < windowPos.y + windowSize.y)
				return true;

			return false;
		}


	};

	enum class ObjectType
	{
		NONE = 0,
		GAMEOBJECT = 1,
		SHADER = 2,
		MODEL = 3,
		MATERIAL = 4,
		SOUND = 5,
		TEXTURE = 6,
		ANIMATION = 7,
		SKYBOX = 8
	};

	class IEditorWindowInterraction : public IEditorWindow
	{
	public:
		~IEditorWindowInterraction() {};
		virtual void Update() = 0;
		void CleanItem()
		{
			m_hasItemSelected = false;
		}
		bool HasItemSelected() { return m_hasItemSelected; }
		virtual void* GetItemSelected(ObjectType& type) = 0;
	protected:

		bool m_hasItemSelected = false;
		void* m_pData = nullptr;
		ObjectType m_type = ObjectType::NONE;

		
	};
}

