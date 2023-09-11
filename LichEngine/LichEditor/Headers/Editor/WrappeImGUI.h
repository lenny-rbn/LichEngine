#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <maths.h>


namespace Physics
{
	class Transform;
	using SetterVec3 = void (Physics::Transform::*) (const Vector3D&);

};

typedef GLFWwindow LichWindow;

namespace EditorUI
{

	typedef ImTextureID TextureID;
	typedef ImGuiWindowFlags WindowFlags;
	typedef ImGuiDockNodeFlags DockingFlags;
	typedef ImGuiTreeNodeFlags Tree_Flags;
	typedef ImGuiCol ColorStyle;
	typedef ImGuiStyleVar VariableStyle;
	typedef ImGuiConfigFlags ConfigFlags;
	typedef ImGuiID IDSystem;
	typedef ImGuiStyle EditorStyle;

	
	struct ViewportUI
	{
		Vector2D size;
		Vector2D pos;
		unsigned int ID; 
	};
	

	struct IOSystem
	{
		ConfigFlags configFlags;
		bool viewportDecoration;
	};

	enum ColorObject
	{
		ButtonCol = ImGuiCol_Button,
		ButtonHovered = ImGuiCol_ButtonHovered,
		CheckMark = ImGuiCol_CheckMark,
		WindowBackground = ImGuiCol_WindowBg,
		ChildBg = ImGuiCol_ChildBg,
		CheckBackground = ImGuiCol_FrameBg, 
		Border = ImGuiCol_Border,
		FrameBgHovered = ImGuiCol_FrameBgHovered,
		FrameBgActive = ImGuiCol_FrameBgActive,
		FrameBg = ImGuiCol_FrameBg,
		HeaderBg = ImGuiCol_Header,
		HeaderActiveBg = ImGuiCol_HeaderActive,
		HeaderHovered = ImGuiCol_HeaderHovered,
		Text = ImGuiCol_Text,
		TextDisabled = ImGuiCol_TextDisabled,
		Tab = ImGuiCol_Tab,
		TabActive = ImGuiCol_TabActive,
		TabUnfocused = ImGuiCol_TabUnfocused,
		TabUnfocusedActive = ImGuiCol_TabUnfocusedActive,
		PopupBg = ImGuiCol_PopupBg,
		BorderShadow = ImGuiCol_BorderShadow,
		TitleBg = ImGuiCol_TitleBg,
		TitleBgCollapsed = ImGuiCol_TitleBgCollapsed,
		TitleBgActive = ImGuiCol_TitleBgActive,
		MenuBarBg = ImGuiCol_MenuBarBg,
		ScrollbarBg = ImGuiCol_ScrollbarBg,
		ScrollbarGrab = ImGuiCol_ScrollbarGrab,
		ScrollbarGrabHovered = ImGuiCol_ScrollbarGrabHovered,
		ScrollbarGrabActive = ImGuiCol_ScrollbarGrabActive,
		SliderGrab = ImGuiCol_SliderGrab,
		SliderGrabActive = ImGuiCol_SliderGrabActive,
		ButtonActive = ImGuiCol_ButtonActive,
		ResizeGrip = ImGuiCol_ResizeGrip,
		ResizeGripHovered = ImGuiCol_ResizeGripHovered,
		ResizeGripActive = ImGuiCol_ResizeGripActive,
		PlotLines = ImGuiCol_PlotLines,
		PlotLinesHovered = ImGuiCol_PlotLinesHovered,
		PlotHistogram = ImGuiCol_PlotHistogram,
		PlotHistogramHovered = ImGuiCol_PlotHistogramHovered,
		TextSelectedBg = ImGuiCol_TextSelectedBg,

	};

	enum VariableStyleFlags
	{
		IndentSpace = ImGuiStyleVar_IndentSpacing,
		FrameBorderSize = ImGuiStyleVar_FrameBorderSize,
	};

	enum StyleType
	{
		Color = 1<<0,
		Var =  1<<1,
	};
	enum class TreeOptions
	{
		OpenArrow = ImGuiTreeNodeFlags_OpenOnArrow,
		OpenDoubleClick = ImGuiTreeNodeFlags_OpenOnDoubleClick,
		SpanAvailWidth = ImGuiTreeNodeFlags_SpanAvailWidth,
		OpenAtStart = ImGuiTreeNodeFlags_DefaultOpen,
		TreeFramed = ImGuiTreeNodeFlags_Framed,
		Leaf = ImGuiTreeNodeFlags_Leaf,
	};

	inline int operator|(TreeOptions a,TreeOptions b) {
		return(int(a) | int(b));
	};
	inline int operator|(int a,TreeOptions b) {
		return(a | int(b));
	};



	enum  WindowOptions
	{
		MenuBar = ImGuiWindowFlags_MenuBar,
		ChildMenu = ImGuiWindowFlags_ChildMenu,
		NoTitleBar = ImGuiWindowFlags_NoTitleBar,
		NoDocking = ImGuiWindowFlags_NoDocking,
		NoCollapse = ImGuiWindowFlags_NoCollapse,
		NoResize = ImGuiWindowFlags_NoResize,
		NoMove = ImGuiWindowFlags_NoMove,
		NoBringToFrontOnFocus = ImGuiWindowFlags_NoBringToFrontOnFocus,
		NoNavFocus = ImGuiWindowFlags_NoNavFocus,
		NoBackground = ImGuiWindowFlags_NoBackground,
		NoScrollBar = ImGuiWindowFlags_NoScrollbar,
		NoDecoration = ImGuiWindowFlags_NoDecoration,
		NoScrollWithMouse = ImGuiWindowFlags_NoScrollWithMouse,
	};

	enum ConfigOption
	{
		Docking_Enable = ImGuiConfigFlags_DockingEnable,
		ViewportsEnable = ImGuiConfigFlags_ViewportsEnable,
		NavEnableKeyboard = ImGuiConfigFlags_NavEnableKeyboard,
	};


	enum  MouseButtonUI
	{
		RightButton = ImGuiMouseButton_Right,
		LeftButton = ImGuiMouseButton_Left,
		MiddleButton = ImGuiMouseButton_Middle,
	};

	enum class TypeUI
	{
		Window = 0,
		TreeNode = 1,
		ChildWindow = 2,
		Popup = 3,
		MenuBar = 4,
		Menu = 5,

	};

	enum ActionType
	{
		Clicked = 1 << 0,
		Hovered = 1 << 1,
		Selected = 1 << 2,
		Active = 1 << 3,
	};

	Vector2D ToVec2(UiVec2D a);
	Vector4D ToVec4(UiVec4D a);
	Vector2D GetWindowSize();
	Vector2D GetMousePos();
	bool SliderVec2(Vector2D* vector,const char* name,const float sensibility = 1.0f,const float min = 0.0f,const float max = 0.0f,const char* format = "%.3f");
	bool SliderVec3(Vector3D* vector,const char* name,const float sensibility = 1.0f,const float min = 0.0f,const float max = 0.0f,const char* format = "%.3f");
	bool ColorField(Vector4D* vector,const char* name);
	bool IntField(int* number,const char* name);
	bool FloatField(float* number,const char* name);
	bool BooleanField(bool* state,const char* name);
	bool Checkbox(bool* state,const char* name);
	bool TextField(const char* name,std::string& input,unsigned const int maxChar);
	bool TextField(const char* name, char* input,unsigned const int maxChar);
	bool SelectableField(const char* name,const char* content,MouseButtonUI inputBind );
	bool SelectableField(const char* name,const char* content);
	void ImageField(TextureID id, Vector2D size , Vector2D uv0 =  Vector2D(0,0),Vector2D uv1 = Vector2D(1,1), Vector4D colorImage = Vector4D(1,1,1,1),Vector4D borderColor = Vector4D(0,0,0,0));
	
	void Indent(const int space = 0);
	void Unindent(const int space = 0);
	void VerticalIndent(const float space = 0);
	void SameLine();
	void Separator();
	bool CreateUI(TypeUI uiType, const char* name ="",bool* open = nullptr,WindowFlags flags = 0);
	bool CreateUI(TypeUI uiType,Tree_Flags flags ,const char* name ="");
	
	void End(TypeUI type,const char* file,int line );
	void Close(TypeUI type);
	bool CreateWindowUI(const char* name,bool* state,WindowFlags flags);
	bool CreateTreeNode(const char* name,Tree_Flags flags);
	bool CreatePopup(const char* text);
	bool CreatePopupWindow(const char* text);
	bool CreateMenuItem(const char* name);
	bool CreateComboField(const char* name,int& index,const char* const* items,int size);

	void CallPopup(const char* name);
	Vector2D GetItemPos();

	void WriteText(const char* text);
	void WriteColorText(std::string text,Vector4D color);
	bool Button(const char* name,Vector2D size = Vector2D(0,0),bool invisible = false,MouseButtonUI input = MouseButtonUI::LeftButton);
	bool Button(const char* name, const char* id,Vector2D size = Vector2D(0,0),bool invisible = false,MouseButtonUI input = MouseButtonUI::LeftButton);
	bool ImageButton(EditorUI::TextureID id,Vector2D size);
	void SetWindowPos(Vector2D position);
	Vector2D GetWindowPos();
	bool IsMouseButtonRelease(MouseButtonUI button);
	bool IsMouseButtonDoubleClic(MouseButtonUI button);
	bool ItemAction(ActionType type,MouseButtonUI input = MouseButtonUI::LeftButton);
	void AlignItem();
	void CenterObject(float widthSize);
	void CenterObject(const char* text);
	Vector2D GetWindowSizeAvailable();

	void PushStyle(ColorStyle colorObject, Vector4D color);
	void PushStyle(VariableStyle varStyle, float value);
	void PopStyle(StyleType type);

	IOSystem* GetIO();
	EditorUI::EditorStyle* GetStyle() { return &ImGui::GetStyle(); };
	void AddFont(const char* path, unsigned int pixelSize, int indexFont);
	IDSystem GetID(const char* idName);

	void CreateDockingSpace( Vector2D size = Vector2D(0,0));

	ViewportUI* GetViewport();
	void SetNextWindowProperty(Vector2D size,Vector2D pos);
	void SetNextWindowViewport(unsigned int id);

	// Manage System UI function
	void ShutdownUISystem();
	void StartUISystem(IOSystem* io, LichWindow* window);
	void CreateContext();
	void SetStyleUIApplication();
	void StartFrame();
	void EndFrame();

	
	void SetStyle();


}