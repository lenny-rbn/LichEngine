#include <pch.h>
#include <Core/Window.h>
#include <ToolBox.h>
namespace Core
{
	Window::Window()
	{
		m_pWindow = nullptr;
		m_pAppSettings = nullptr;
	}

	Window::~Window() {}

	void Window::SetAppSettings(AppSettings* settings)
	{
		m_pAppSettings = settings;
	}

	void Window::InitWindow(AppSettings settings)
	{
		m_pWindow = WindowAPI::Init(settings.width, settings.height, settings.name.c_str());
#ifdef  LICH_GAME
		WindowAPI::SetInputMode(m_pWindow, Input::CURSOR, Mode::DISABLED);
#endif //  LICHGAME

	}

	void Window::Update()
	{
		Vector2D currentSize = { m_pAppSettings->width ,m_pAppSettings->height };

		Vector2D size = WindowAPI::GetWindowSize(m_pWindow);
		if (currentSize != size)
		{
			m_pAppSettings->SetWindowSize(size);
			for (std::vector<Core::Editor::WindowSubscriber*>::iterator iter = m_windowSizeSubscribers.begin(); iter != m_windowSizeSubscribers.end(); iter++)
			{
				(*iter)->SetWindowSize(size);
			}
		
		}
	}

	void Window::SubscribeSizeWindowEvent(Core::Editor::WindowSubscriber* sub)
	{
		if (!Utils::IsInVector<Core::Editor::WindowSubscriber*>(m_windowSizeSubscribers, sub))
		{
			m_windowSizeSubscribers.push_back(sub);
		}
	}

	void Window::ApplyChange()
	{
		m_pAppSettings->SetWindowSize(GetWindowSize());
	}

	Vector2D Window::GetWindowSize()
	{
		return WindowAPI::GetWindowSize(m_pWindow);
	}

	void Window::Destroy()
	{
		WindowAPI::Terminate();
	}
}