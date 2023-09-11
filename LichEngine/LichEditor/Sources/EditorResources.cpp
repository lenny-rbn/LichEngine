
#include <Resources/ResourceManager.h>
#include <Editor/EditorResources.h>

EditorLich::EditorResources::EditorResources()
{
}

EditorLich::EditorResources::~EditorResources()
{
}

void EditorLich::EditorResources::LoadFont()
{
	EditorUI::AddFont(".\\EditorResources\\Fonts\\NunitoSans-Bold.ttf", 16,0);
}

void EditorLich::EditorResources::ApplyEditorGraphicStyle()
{
	EditorUI::EditorStyle* style = EditorUI::GetStyle();

	style->WindowPadding = EditorUI::ToVec2(Vector2D(15,15));
	style->WindowRounding = 5.0f;
	style->FramePadding = EditorUI::ToVec2(Vector2D(5,5));
	style->FrameRounding = 4.0f;
	style->ItemSpacing = EditorUI::ToVec2(Vector2D(12,8));
	style->ItemInnerSpacing = EditorUI::ToVec2(Vector2D(8,6));
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[EditorUI::ColorObject::Text] = EditorUI::ToVec4(Vector4D(0.80f,0.80f,0.83f,1.00f));

	style->Colors[EditorUI::ColorObject::Tab] = EditorUI::ToVec4(Vector4D(0.50f, 0.30f, 0.60f, 0.40f));
	style->Colors[EditorUI::ColorObject::TabActive] = EditorUI::ToVec4(Vector4D(0.50f, 0.30f, 0.60f, 0.80f));
	style->Colors[EditorUI::ColorObject::TabUnfocused] = EditorUI::ToVec4(Vector4D(0.50f, 0.30f, 0.60f, 0.40f));
	style->Colors[EditorUI::ColorObject::TabUnfocusedActive] = EditorUI::ToVec4(Vector4D(0.50f, 0.30f, 0.60f, 0.80f));

	style->Colors[EditorUI::ColorObject::TextDisabled] = EditorUI::ToVec4(Vector4D(0.24f,0.23f,0.29f,1.00f));
	style->Colors[EditorUI::ColorObject::WindowBackground] = EditorUI::ToVec4(Vector4D(0.06f,0.05f,0.07f,1.00f));
	style->Colors[EditorUI::ColorObject::ChildBg] = EditorUI::ToVec4(Vector4D(0.07f,0.07f,0.09f,1.00f));
	style->Colors[EditorUI::ColorObject::PopupBg] = EditorUI::ToVec4(Vector4D(0.07f,0.07f,0.09f,1.00f));
	style->Colors[EditorUI::ColorObject::Border] = EditorUI::ToVec4(Vector4D(0.80f,0.80f,0.83f,0.88f));
	style->Colors[EditorUI::ColorObject::BorderShadow] = EditorUI::ToVec4(Vector4D(0.92f,0.91f,0.88f,0.00f));
	style->Colors[EditorUI::ColorObject::FrameBg] = EditorUI::ToVec4(Vector4D(0.19f,0.19f,0.19f,1.00f));
	style->Colors[EditorUI::ColorObject::FrameBgHovered] = EditorUI::ToVec4(Vector4D(0.24f,0.23f,0.29f,1.00f));
	style->Colors[EditorUI::ColorObject::FrameBgActive] = EditorUI::ToVec4(Vector4D(0.56f,0.56f,0.58f,1.00f));
	style->Colors[EditorUI::ColorObject::TitleBg] = EditorUI::ToVec4(Vector4D(0.10f,0.09f,0.12f,1.00f));
	style->Colors[EditorUI::ColorObject::TitleBgCollapsed] = EditorUI::ToVec4(Vector4D(1.00f,0.98f,0.95f,0.75f));
	style->Colors[EditorUI::ColorObject::TitleBgActive] = EditorUI::ToVec4(Vector4D(0.07f,0.07f,0.09f,1.00f));
	style->Colors[EditorUI::ColorObject::MenuBarBg] = EditorUI::ToVec4(Vector4D(0.10f,0.09f,0.12f,1.00f));
	style->Colors[EditorUI::ColorObject::ScrollbarBg] = EditorUI::ToVec4(Vector4D(0.10f,0.09f,0.12f,1.00f));
	style->Colors[EditorUI::ColorObject::ScrollbarGrab] = EditorUI::ToVec4(Vector4D(0.80f,0.80f,0.83f,0.31f));
	style->Colors[EditorUI::ColorObject::ScrollbarGrabHovered] = EditorUI::ToVec4(Vector4D(0.56f,0.56f,0.58f,1.00f));
	style->Colors[EditorUI::ColorObject::ScrollbarGrabActive] = EditorUI::ToVec4(Vector4D(0.06f,0.05f,0.07f,1.00f));
	style->Colors[EditorUI::ColorObject::CheckMark] = EditorUI::ToVec4(Vector4D(0.80f,0.80f,0.83f,0.61f));
	style->Colors[EditorUI::ColorObject::SliderGrab] = EditorUI::ToVec4(Vector4D(0.80f,0.80f,0.83f,0.31f));
	style->Colors[EditorUI::ColorObject::SliderGrabActive] = EditorUI::ToVec4(Vector4D(0.06f,0.05f,0.07f,1.00f));
	style->Colors[EditorUI::ColorObject::ButtonCol] = EditorUI::ToVec4(Vector4D(0.15f,0.15f,0.15f,1.00f));
	style->Colors[EditorUI::ColorObject::ButtonHovered] = EditorUI::ToVec4(Vector4D(0.24f,0.23f,0.29f,1.00f));
	style->Colors[EditorUI::ColorObject::ButtonActive] = EditorUI::ToVec4(Vector4D(0.56f,0.56f,0.58f,1.00f));
	style->Colors[EditorUI::ColorObject::HeaderBg] = EditorUI::ToVec4(Vector4D(0.85f, 0.30f, 0.92f, 0.3f)); //
	style->Colors[EditorUI::ColorObject::HeaderHovered] = EditorUI::ToVec4(Vector4D(0.24f, 0.23f, 0.29f, 1.00f));
	style->Colors[EditorUI::ColorObject::HeaderActiveBg] = EditorUI::ToVec4(Vector4D(0.06f,0.05f,0.07f,1.00f));
	style->Colors[EditorUI::ColorObject::ResizeGrip] = EditorUI::ToVec4(Vector4D(0.00f,0.00f,0.00f,0.00f));
	style->Colors[EditorUI::ColorObject::ResizeGripHovered] = EditorUI::ToVec4(Vector4D(0.56f,0.56f,0.58f,1.00f));
	style->Colors[EditorUI::ColorObject::ResizeGripActive] = EditorUI::ToVec4(Vector4D(0.06f,0.05f,0.07f,1.00f));
	style->Colors[EditorUI::ColorObject::PlotLines] = EditorUI::ToVec4(Vector4D(0.40f,0.39f,0.38f,0.63f));
	style->Colors[EditorUI::ColorObject::PlotLinesHovered] = EditorUI::ToVec4(Vector4D(0.25f,1.00f,0.00f,1.00f));
	style->Colors[EditorUI::ColorObject::PlotHistogram] = EditorUI::ToVec4(Vector4D(0.40f,0.39f,0.38f,0.63f));
	style->Colors[EditorUI::ColorObject::PlotHistogramHovered] = EditorUI::ToVec4(Vector4D(0.25f,1.00f,0.00f,1.00f));
	style->Colors[EditorUI::ColorObject::TextSelectedBg] = EditorUI::ToVec4(Vector4D(0.25f,1.00f,0.00f,0.43f));
}

void EditorLich::EditorResources::InitGraphicsResource()
{
	LoadGrahicsResources();
}


void EditorLich::EditorResources::LoadGrahicsResources()
{
	for(auto& p : std::filesystem::recursive_directory_iterator(m_editorResoucePath))
	{
		std::string name = p.path().filename().string();
		name = name.substr(0,name.find_first_of('.'));
		if (p.path().extension() == ".jpg" || p.path().extension() == ".png")
		{
			Resources::Create(Resources::ResourceType::TEXTURE, p.path().string(),name,Resources::ResourceLayer::EDITOR);
		}
	}

	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/GizmoCircle/gizmoRotation.obj", "GizmoCircle", Resources::ResourceLayer::EDITOR);
	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/GizmoArrow/gizmoArrow.obj", "GizmoArrow", Resources::ResourceLayer::EDITOR);
	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/GizmoScale/GizmoScale.obj", "GizmoScale", Resources::ResourceLayer::EDITOR);
	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/Sphere.obj", "Sphere", Resources::ResourceLayer::EDITOR);
	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/Cube/cube.obj", "Cube", Resources::ResourceLayer::EDITOR);
	Resources::Create(Resources::ResourceType::MODEL, "EditorResources/Capsule/Capsule.obj", "Capsule", Resources::ResourceLayer::EDITOR);
}
