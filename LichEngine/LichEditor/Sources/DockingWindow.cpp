#include <ToolBox.h>
#include <Core/App.h>
#include <Editor/HierachyWindow.h>
#include <Editor/FileBrowserWindow.h>
#include <Editor/InspectorWindow.h>
#include <Editor/GameViewport.h>
#include <Editor/DockingWindow.h>
#include <Editor/ConsoleWindow.h>
#include <chrono>
#include <Core/TimeManager.h>

using namespace EditorLich;

DockingWindow::DockingWindow(EngineData* engineData, Core::App* app)
{
	const EditorUI::ViewportUI* viewport = EditorUI::GetViewport();
	EditorUI::SetNextWindowProperty(viewport->size, viewport->pos);
	EditorUI::SetNextWindowViewport(viewport->ID);

	m_window_flags = EditorUI::WindowOptions::MenuBar | EditorUI::WindowOptions::NoDocking;
	m_window_flags |= EditorUI::WindowOptions::NoTitleBar | EditorUI::WindowOptions::NoCollapse | EditorUI::WindowOptions::NoResize | EditorUI::WindowOptions::NoMove;
	m_window_flags |= EditorUI::WindowOptions::NoBringToFrontOnFocus | EditorUI::WindowOptions::NoNavFocus;
	m_window_flags |= EditorUI::WindowOptions::NoBackground;

	m_pWindow = app->GetWindow();
	m_pWindow->SubscribeSizeWindowEvent(this);

	CreateEngineWindow(engineData, app);
}

DockingWindow::~DockingWindow()
{
	for (auto p : m_editorWindow)
		delete p;

	m_editorWindow.clear();
}

void EditorLich::DockingWindow::Update()
{
	UpdateWindowSize();

	if (EditorUI::ToVec2(EditorUI::GetViewport()->size) != Vector2D(0,0))
	{
		EditorUI::CreateUI(EditorUI::TypeUI::Window,m_windowName,&m_isOpen,m_window_flags);

		HandleDockingSpace();
		EngineMenuBar();
		UpdateEngineWindow();
		EditorUI::End(EditorUI::TypeUI::Window,__FILE__,__LINE__);
	}
}


void EditorLich::DockingWindow::AddWindow(IEditorWindow* window)
{
	if (!Utils::IsInVector<IEditorWindow*>(m_editorWindow,window))
		m_editorWindow.push_back(window);
}

void EditorLich::DockingWindow::SetWindowSize(const Vector2D size)
{
	m_currentSize = size;
	m_isChange = true;
}

void EditorLich::DockingWindow::CreateEngineWindow(EngineData* engineData, Core::App* app)
{

	GameViewport* gameWindow = new GameViewport(engineData->pRenderManager, app, engineData->pSceneManager);
	AddWindow((IEditorWindow*)gameWindow);

	FileBrowser* fileBrowser = new FileBrowser(&m_assetResources, engineData->pResouceManager, engineData->pSceneManager);
	AddWindow((IEditorWindow*)fileBrowser);

	HierachyWindow* hierachyWindow = new HierachyWindow(engineData->pSceneManager, fileBrowser);
	AddWindow((IEditorWindow*)hierachyWindow);
	gameWindow->SetHierarchyWindow(hierachyWindow);
	hierachyWindow->SetCurrentSceneGraph((engineData->pSceneManager->GetCurrentScenePtr()));

	InspectorWindow* inspectorWindow = new InspectorWindow(&m_assetResources);
	AddWindow((IEditorWindow*)inspectorWindow);

	inspectorWindow->AddInteractableWindow(hierachyWindow);
	inspectorWindow->AddInteractableWindow(fileBrowser);
	ConsoleWindow* consoleWindow = new ConsoleWindow();
	AddWindow((IEditorWindow*)consoleWindow);

	m_pSceneManager = engineData->pSceneManager;
}

void EditorLich::DockingWindow::UpdateWindowSize()
{
	const EditorUI::ViewportUI* viewport = EditorUI::GetViewport();
	EditorUI::SetNextWindowProperty(viewport->size, viewport->pos);
	EditorUI::SetNextWindowViewport(viewport->ID);
	m_isChange = false;
}

void EditorLich::DockingWindow::EngineMenuBar()
{
	if (EditorUI::CreateUI(EditorUI::TypeUI::MenuBar))
	{
		if (EditorUI::CreateUI(EditorUI::TypeUI::Menu, m_fileMenuName))
		{
			if (ImGui::MenuItem("Create Scene"))
			{
				m_pSceneManager->CreateNewScene("", "");
			}

			EditorUI::End(EditorUI::TypeUI::Menu,__FILE__,__LINE__);
		}

		if (EditorUI::Button("Save", Vector2D(2, 10)))
			m_pSceneManager->SaveCurrentScene();

		EditorUI::End(EditorUI::TypeUI::MenuBar, __FILE__, __LINE__);
	}
}

void EditorLich::DockingWindow::HandleDockingSpace()
{
	EditorUI::IOSystem* io = EditorUI::GetIO();
	if (io->configFlags & EditorUI::ConfigOption::Docking_Enable)
		EditorUI::CreateDockingSpace();
}


void EditorLich::DockingWindow::UpdateEngineWindow()
{
	for (std::vector<IEditorWindow*>::iterator iter = m_editorWindow.begin(); iter != m_editorWindow.end(); iter++)
		(*iter)->Update();
}