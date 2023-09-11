#include<Physics/Transform.h>
#include <Debugs/Log.h>
#include <Editor/WrappeImGUI.h>
#include <Core/WrapperWindow.h>

Vector2D EditorUI::ToVec2(EditorUI::UiVec2D a)
{
	return Vector2D(a.x, a.y);
}

Vector4D EditorUI::ToVec4(UiVec4D a)
{
	return Vector4D(a.x,a.y,a.z,a.w);
}

Vector2D EditorUI::GetWindowSize()
{
	return ToVec2(ImGui::GetWindowSize());
}

Vector2D EditorUI::GetMousePos()
{
	return ToVec2(ImGui::GetMousePos());
}

bool EditorUI::SliderVec2(Vector2D* vector, const char* name, const float sensibility, const float min, const float max, const char* format)
{
	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::DragFloat2("", (*vector).coord, sensibility, min, max, format))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool  EditorUI::SliderVec3(Vector3D* vector, const char* name, const float sensibility, const float min, const float max, const char* format)
{

	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::DragFloat3("", (*vector).coord, sensibility, min, max, format))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;

}

bool EditorUI::ColorField(Vector4D* vector, const char* name)
{
	ImGuiColorEditFlags flags = 0;

	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::ColorPicker4("", (*vector).coord, flags))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool EditorUI::IntField(int* number, const char* name)
{
	ImGui::AlignTextToFramePadding();
	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::InputInt("", number))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool EditorUI::FloatField(float* number, const char* name)
{
	ImGui::AlignTextToFramePadding();
	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::InputFloat("", number))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool EditorUI::Checkbox(bool* state, const char* name)
{
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::Checkbox("", state))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool EditorUI::BooleanField(bool* state, const char* name)
{
	ImGui::Text(name);
	ImGui::PushID(name + 1);
	ImGui::SameLine();
	if (ImGui::Checkbox("", state))
	{
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();
	return false;
}

bool EditorUI::TextField(const char* name, std::string& input, unsigned const int maxChar)
{
	ImGui::AlignTextToFramePadding();
	if (name != "")
	{
		ImGui::Text(name);
		ImGui::SameLine();
	}
	ImGui::PushID("#item");
	input.resize(maxChar, 0);
	bool state = ImGui::InputText("", &input[0], maxChar);
	input.shrink_to_fit();
	ImGui::PopID();
	return  state;
}

bool EditorUI::TextField(const char* name, char* input, unsigned const int maxChar)
{
	ImGui::AlignTextToFramePadding();
	if (name != "")
	{
		ImGui::Text(name);
		ImGui::SameLine();
	}
	ImGui::PushID("#item");
	bool state = ImGui::InputText("",&input[0],maxChar);
	ImGui::PopID();
	return  state;
}

bool EditorUI::SelectableField(const char* name, const char* content, MouseButtonUI inputBind)
{
	ImGui::AlignTextToFramePadding();
	if (name != "")
	{
		ImGui::Text(name);
		ImGui::SameLine();
	}
	ImGui::PushID("#item");
	ImGui::Selectable(content);
	ImGui::PopID();
	if (ImGui::IsItemClicked((ImGuiMouseButton)inputBind))
	{
		return true;
	}
	return false;
}

bool EditorUI::SelectableField(const char* name, const char* content)
{
	ImGui::AlignTextToFramePadding();
	if (name != "")
	{
		ImGui::Text(name);
		ImGui::SameLine();
	}
	ImGui::PushID("#item");
	bool state = ImGui::Selectable(content);
	ImGui::PopID();
	return state;
}

void EditorUI::Indent(const int space)
{
	if (space == 0)
	{
		ImGui::Indent();
		return;
	}
	else
	{
		ImGui::Dummy(Vector2D(space, 0));
	}
}

void EditorUI::Unindent(const int space)
{
	if (space == 0)
	{
		ImGui::Unindent();
		return;
	}
	else
	{
		ImGui::Dummy(Vector2D(-space, 0));
	}
}

void EditorUI::VerticalIndent(const float space)
{
	if (space == 0)
	{
		ImGui::NewLine();
	}
	else
	{
		ImGui::Dummy(Vector2D(0.0f, space));
	}
}

void EditorUI::SameLine()
{
	ImGui::SameLine();
}

void EditorUI::Separator()
{
	ImGui::Separator();
}

bool EditorUI::CreateUI(TypeUI uiType, const char* name, bool* open, WindowFlags flags)
{
	switch (uiType)
	{
	case EditorUI::TypeUI::Window:
		return ImGui::Begin(name, open, flags);
		break;
	case EditorUI::TypeUI::TreeNode:
		return ImGui::TreeNodeEx(name, (Tree_Flags)0);
		break;
	case EditorUI::TypeUI::ChildWindow:
		return ImGui::BeginChild(name, ImVec2(0, 0), false, flags);
		break;
	case EditorUI::TypeUI::Popup:
		return EditorUI::CreatePopup(name);
		break;
	case EditorUI::TypeUI::MenuBar:
		return ImGui::BeginMenuBar();
		break;
	case EditorUI::TypeUI::Menu:
		return ImGui::BeginMenu(name);
		break;
	default:
		return false;
		break;
	}
}

bool EditorUI::CreateUI(TypeUI uiType, Tree_Flags flags, const char* name)
{
	switch (uiType)
	{
	case EditorUI::TypeUI::Window:
		return ImGui::Begin(name);
		break;
	case EditorUI::TypeUI::TreeNode:
		return ImGui::TreeNodeEx(name, flags);
		break;
	case EditorUI::TypeUI::ChildWindow:
		return ImGui::BeginChild(name);
		break;
	case EditorUI::TypeUI::Popup:
		return EditorUI::CreatePopup(name);
		break;
	case EditorUI::TypeUI::MenuBar:
		return ImGui::BeginMenuBar();
		break;
	default:
		return false;
		break;

	}
}

bool EditorUI::CreateWindowUI(const char* name, bool* state, WindowFlags flags)
{
	return ImGui::Begin(name, state, flags);
}

void EditorUI::End(TypeUI type, const char* path, int line)
{
	std::string lineC = " " + std::to_string(line);

	switch (type)
	{
	case EditorUI::TypeUI::Window:

#ifdef WindowEditorDebug
		LDEBUG(path + lineC + " window is close");
#endif // WindowEditorDebug

		ImGui::End();
		break;
	case EditorUI::TypeUI::TreeNode:
		ImGui::TreePop();
		break;
	case EditorUI::TypeUI::ChildWindow:
		ImGui::EndChild();
		break;
	case EditorUI::TypeUI::Popup:
		ImGui::EndPopup();
		break;
	case EditorUI::TypeUI::MenuBar:
		ImGui::EndMenuBar();
		break;
	case EditorUI::TypeUI::Menu:
		ImGui::EndMenu();
		break;
	default:
		break;
	}
}

void EditorUI::Close(TypeUI type)
{
	switch (type)
	{
	case EditorUI::TypeUI::Window:
		break;
	case EditorUI::TypeUI::TreeNode:
		break;
	case EditorUI::TypeUI::ChildWindow:
		break;
	case EditorUI::TypeUI::Popup:
		ImGui::CloseCurrentPopup();
		break;
	default:
		break;
	}
}

bool EditorUI::CreateTreeNode(const char* name, Tree_Flags flags)
{
	return ImGui::TreeNodeEx(name, flags);
}

void EditorUI::WriteText(const char* text)
{
	ImGui::Text(text);
}

void EditorUI::WriteColorText(std::string text, Vector4D color)
{
	if (text == "") return;

	EditorUI::PushStyle(EditorUI::ColorObject::Text, color);
	EditorUI::WriteText(text.c_str());
	EditorUI::PopStyle(EditorUI::StyleType::Color);

}

bool EditorUI::CreatePopup(const char* text)
{

	return ImGui::BeginPopup(text);
}

bool EditorUI::CreatePopupWindow(const char* text)
{
	return ImGui::BeginPopupContextWindow(text);
}

bool EditorUI::CreateComboField(const char* name, int& index, const char* const* items, int size)
{
	return ImGui::Combo(name, &index, items, size);
}

void EditorUI::CallPopup(const char* name)
{
	ImGui::OpenPopup(name);
}

Vector2D EditorUI::GetItemPos()
{
	return ToVec2(ImGui::GetItemRectMin());
}

bool EditorUI::Button(const char* name, Vector2D size, bool invisible, MouseButtonUI input)
{

	if (size == Vector2D::zero && invisible) return false;

	if (invisible)
		return ImGui::InvisibleButton(name, size, input);
	else
	{
		ImGui::PushID(name + 1);
		bool state = ImGui::Button(name);
		ImGui::PopID();
		return state;
	}

	 
}


bool EditorUI::Button(const char* name, const char* id, Vector2D size, bool invisible, MouseButtonUI input)
{

	if (size == Vector2D::zero && invisible) return false;

	if (invisible)
		return ImGui::InvisibleButton(name, size, input);
	else
	{
		ImGui::PushID(id);
		bool state = ImGui::Button(name);
		ImGui::PopID();
		return state;
	}


}


bool EditorUI::ImageButton(EditorUI::TextureID id , Vector2D size)
{
	return 	ImGui::ImageButton(id,size);
}

void EditorUI::SetWindowPos(Vector2D position)
{
	return ImGui::SetWindowPos(position);
}

Vector2D EditorUI::GetWindowPos()
{
	return ToVec2(ImGui::GetWindowPos());
}

bool EditorUI::CreateMenuItem(const char* name)
{
	return ImGui::MenuItem(name, NULL, false);
}

bool EditorUI::IsMouseButtonRelease(MouseButtonUI button)
{
	return ImGui::IsMouseReleased(button);
}

bool EditorUI::IsMouseButtonDoubleClic(MouseButtonUI button)
{
	return ImGui::IsMouseDoubleClicked(button);
}



bool EditorUI::ItemAction(ActionType type, MouseButtonUI input)
{
	switch (type)
	{
	case EditorUI::Clicked:
		return ImGui::IsItemClicked(input);
		break;
	case EditorUI::Hovered:
		return ImGui::IsItemHovered();
		break;
	case EditorUI::Selected:
		return ImGui::IsItemFocused();
		break;
	case EditorUI::Active:
		return ImGui::IsItemActive();
		break;
	default:
		break;
	}
	return false;
}

void EditorUI::AlignItem()
{
	ImGui::AlignTextToFramePadding();
}

void EditorUI::CenterObject(float widthSize)
{
	auto windowWidth = ImGui::GetWindowSize().x;
	ImGui::SetCursorPosX((windowWidth - widthSize) * 0.5f);
}

void EditorUI::CenterObject(const char* text)
{
	auto textWidth = ImGui::CalcTextSize(text).x;
	auto windowWidth = ImGui::GetWindowSize().x ;
	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
}

Vector2D EditorUI::GetWindowSizeAvailable()
{
	return ToVec2(ImGui::GetContentRegionAvail());
}

void EditorUI::PushStyle(ColorStyle colorObject, Vector4D color)
{
	ImGui::PushStyleColor(colorObject, color);
}

void EditorUI::PushStyle(VariableStyle varStyle, float value)
{
	ImGui::PushStyleVar(varStyle, value);
}

void EditorUI::PopStyle(StyleType type)
{
	switch (type)
	{
	case EditorUI::Color:
		ImGui::PopStyleColor();
		break;
	case EditorUI::Var:
		ImGui::PopStyleVar();
		break;
	default:
		break;
	}
}

EditorUI::IOSystem* EditorUI::GetIO()
{
	EditorUI::IOSystem* systemIO = new EditorUI::IOSystem();
	systemIO->configFlags = ImGui::GetIO().ConfigFlags;
	return systemIO;
}

void EditorUI::AddFont(const char* path, unsigned int pixelSize, int indexFont)
{
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(path, pixelSize);
	io.FontDefault = io.Fonts->Fonts[0];
	
}



EditorUI::IDSystem EditorUI::GetID(const char* idName)
{
	return ImGui::GetID(idName);
}

void EditorUI::CreateDockingSpace(Vector2D size)
{
	EditorUI::IDSystem dockspace_id = EditorUI::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, size);
}

EditorUI::ViewportUI* EditorUI::GetViewport()
{
	ImGuiViewport* view = ImGui::GetMainViewport();
	ViewportUI* viewport = new ViewportUI();
	viewport->size = ToVec2(view->Size);
	viewport->pos = ToVec2(view->Pos);
	viewport->ID = view->ID;
	return viewport;
}

void EditorUI::SetNextWindowProperty(Vector2D size, Vector2D pos)
{
	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);
}

void EditorUI::ImageField(TextureID id, Vector2D size, Vector2D uv0, Vector2D uv1, Vector4D colorImage, Vector4D borderColor)
{
	ImGui::Image((ImTextureID)id, size, uv0, uv1, colorImage, borderColor);
}
void EditorUI::SetNextWindowViewport(unsigned int id)
{
	ImGui::SetNextWindowViewport(id);
}

void EditorUI::ShutdownUISystem()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorUI::CreateContext()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
}

void EditorUI::StartUISystem(EditorUI::IOSystem* ioSystem, LichWindow* window)
{
	const char* glsl_version = "#version 330";

	ImGuiIO* io = &ImGui::GetIO();
	io->ConfigFlags = ioSystem->configFlags;
	io->ConfigViewportsNoDecoration = ioSystem->viewportDecoration;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void EditorUI::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorUI::EndFrame()
{
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void EditorUI::SetStyle()
{
	ImGui::StyleColorsDark();
	
}





