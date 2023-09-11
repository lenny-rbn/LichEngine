#include <ToolBox.h>
#include <Core/App.h>
#include <Core/WrapperWindow.h>
#include <Editor/EditorResources.h>
#include <Core/App.h>
#include <Editor.h>
using namespace EditorLich;


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Editor::Editor()
{
	m_pWindow = nullptr;
	m_pIO = nullptr;
	m_pEngineData = nullptr;
	m_pApp= nullptr;
	m_pDockingWindow = nullptr;
}

Editor::~Editor()
{
	EditorUI::ShutdownUISystem();

	delete m_pApp;
	delete m_pEngineData;
	delete m_pIO;
	delete m_pDockingWindow;
}

void EditorLich::Editor::Init()
{

	InitWindow();
	InitEditor();
	m_pApp->Initialize();

	m_editorResources.InitGraphicsResource();
	GetEngineData();
	CreateEditorChildrenWindow();
}



void EditorLich::Editor::Run()
{
	Init();
	while (!WindowAPI::WindowShouldClose(m_pWindow) )
	{
		Update();
		Draw(m_pWindow);

		WindowAPI::SwipBuffers(m_pWindow);
	}
	Close();
}

void EditorLich::Editor::InitWindow()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return;
	
	m_pApp = new Core::App();
	m_pWindow = m_pApp->GetWindow()->GetWindow();

	WindowAPI::SetCurrentContext(m_pWindow);
	gladLoadGL();
}

void EditorLich::Editor::InitEditor()
{
	EditorUI::CreateContext();

	m_pIO = EditorUI::GetIO();
	m_pIO->viewportDecoration = true;
	m_pIO->configFlags |= EditorUI::ConfigOption::Docking_Enable | EditorUI::ConfigOption::NavEnableKeyboard;
	m_editorResources.LoadFont();	
	m_editorResources.ApplyEditorGraphicStyle();

	EditorUI::StartUISystem(m_pIO,m_pWindow);
	
}

void EditorLich::Editor::GetEngineData()
{
	m_pEngineData = new EngineData();

	const rfk::Field* field = Core::App::staticGetArchetype().getFieldByName("m_pSceneManager");
	m_pEngineData->pSceneManager = field->getUnsafe<SceneManagement::SceneManager*>(m_pApp);
	field = SceneManagement::SceneManager::staticGetArchetype().getFieldByName("m_pRenderManager");
	m_pEngineData->pRenderManager = field->getUnsafe<Renderer::RenderManager*>(m_pEngineData->pSceneManager);
	m_pEngineData->pResouceManager = (*m_pApp->GetResourceManager());
}

void EditorLich::Editor::CreateEditorChildrenWindow()
{
	m_pDockingWindow = new EditorLich::DockingWindow(m_pEngineData, m_pApp);
}


void EditorLich::Editor::Draw(LichWindow* window)
{
	m_pApp->Draw();
	EditorUI::EndFrame();
}

void EditorLich::Editor::Close()
{
	m_pApp->Close();
}


void EditorLich::Editor::Update()
{
	m_pApp->Update();
	EditorUI::StartFrame();
	m_pDockingWindow->Update();
}




