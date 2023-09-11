#include <pch.h>
#include <Core/InputManager.h>


using namespace Core;

Core::InputManager::InputManager(LichWindow* window): m_pCurrentWindow(window) {}

Core::InputManager::~InputManager() {}

void Core::InputManager::Init()
{
	LINFO("Init Input manager ");
	const char* path = "InputConfig.txt";
	LoadInputConfig(path);
}

void Core::InputManager::Update()
{
	UpdateCurrentInputLayout();
}

void Core::InputManager::Destroy()
{
	m_pInputLayout->Destroy();
	delete m_pInputLayout;
}

void Core::InputManager::LoadInputConfig(const char* path)
{
	std::filesystem::path filepath(path);
	std::ifstream file;
	std::string line;
	m_pInputLayout = new InputConfig();
	if (std::filesystem::exists(filepath))
	{
		file.open(filepath.c_str(),std::ifstream::in);
		while (std::getline(file,line))
		{
			InputAction* action = new InputAction();
			size_t posNxt = line.find(',');
			size_t posPrev = 0;
			action->name = line.substr(posPrev,posNxt);
			
			posPrev = posNxt;
			posNxt = line.find(',', posPrev + 1);
			action->source = (Input)std::stoi(line.substr(posPrev + 1, posNxt));

			posPrev = posNxt;
			posNxt = line.find(',',posPrev+1);
			action->touch = (Key)std::stoi(line.substr(posPrev + 1,posNxt));

			m_pInputLayout->actions.insert(std::pair<std::string,InputAction*>(action->name,action));
		}
		LINFO("Input layout has been successfully loaded");
	}
	else
	{
		LERROR("No input file found");
	}
}

void Core::InputManager::UpdateCurrentInputLayout()
{
	for (std::map<std::string,InputAction*>::iterator iter = m_pInputLayout->actions.begin(); iter != m_pInputLayout->actions.end(); iter++)
		CheckKey(iter->second);
}

void Core::InputManager::CheckKey(InputAction* action)
{
	bool isPress = false;
	switch (action->source)
	{
	case Input::CURSOR:
		break;
	case Input::STICKY_KEYS:
		isPress = (WindowAPI::GetKey(m_pCurrentWindow, action->touch) == InputMode::PRESS);
		if (isPress)
		{
			if (action->state == InputState::PRESSED)
				action->state = InputState::CONTINUE;
			if (action->state == InputState::NONE || action->state == InputState::RELEASED)
				action->state = InputState::PRESSED;
		}
		else
		{
			if (action->state == InputState::RELEASED)
				action->state = InputState::NONE;
			if (action->state == InputState::CONTINUE || action->state == InputState::PRESSED)
				action->state = InputState::RELEASED;
		}
		break;
	case Input::STICKY_MOUSE_BUTTONS:
		isPress = (WindowAPI::GetMouseButton(m_pCurrentWindow, action->touch) == InputMode::PRESS);
		if (isPress)
		{
			if (action->state == InputState::PRESSED)
				action->state = InputState::CONTINUE;
			if (action->state == InputState::NONE || action->state == InputState::RELEASED)
				action->state = InputState::PRESSED;
		}
		else
		{
			if (action->state == InputState::RELEASED)
				action->state = InputState::NONE;
			if (action->state == InputState::CONTINUE || action->state == InputState::PRESSED)
				action->state = InputState::RELEASED;
		}
		break;
	case Input::LOCK_KEY_MODS:
		break;
	case Input::RAW_MOUSE_MOTION:
		break;
	default:
		break;
	}
}