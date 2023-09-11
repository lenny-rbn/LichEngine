#pragma once
#include <Core/AppSettings.h>
#include <Core/WrapperWindow.h>
#include <EditorLich/WindowSubscriber.h>


namespace Core
{
	class LICH_ENGINE Window
	{
	private: 
		LichWindow* m_pWindow;
		AppSettings* m_pAppSettings;

		std::vector<Core::Editor::WindowSubscriber*> m_windowSizeSubscribers;
	public:
		Window();
		~Window();

		void Update();
		void Destroy();
		void ApplyChange();
		void InitWindow(AppSettings settings);
		void SetAppSettings(AppSettings* settings);
		
		Vector2D GetWindowSize();
		void SubscribeSizeWindowEvent(Core::Editor::WindowSubscriber* sub);

		inline LichWindow* GetWindow() { return m_pWindow; }
		inline void SetWindow(LichWindow* window) {  m_pWindow = window; }
	};
}