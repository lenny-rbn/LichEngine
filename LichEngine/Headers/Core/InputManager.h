#pragma once
#include <map>

#include <Debugs/Log.h>
#include <Core/WrapperWindow.h>


namespace Core
{
	enum InputState
	{
		PRESSED,
		CONTINUE,
		RELEASED,
		NONE,
	};

	// Stock a input action setup by designer
	LICH_ENGINE struct InputAction
	{
		InputState state = InputState::NONE;

		Key touch;
		Input source;
		std::string name;
	};

	// Contains a input layout for the game
	LICH_ENGINE struct InputConfig
	{
		std::map<std::string,InputAction*> actions;
		void Destroy()
		{
			for (std::map<std::string,InputAction*>::iterator iter = actions.begin(); iter != actions.end(); iter++)
			{
				InputAction* action = iter->second;
				iter->second = nullptr;
				delete(action);
			}
		}
	};

	// Manage inputs from the player and allows to accesss it
	LICH_ENGINE class InputManager
	{
	public:
		InputManager(LichWindow* window);
		~InputManager();

		void Init();
		void Update();
		void Destroy();

		bool IsPressed(const char* name);
		bool IsContinue(const char* name);
		bool IsReleased(const char* name);

		InputState GetInputState(const char* name);
		LichWindow* GetWindow() { return m_pCurrentWindow; }

	private:
		InputConfig* m_pInputLayout;
		LichWindow* m_pCurrentWindow;
	
		void LoadInputConfig(const char* path);
		void UpdateCurrentInputLayout();
		void CheckKey(InputAction* action);
	};

	inline InputState InputManager::GetInputState(const char* name)
	{
		try
		{
			return m_pInputLayout->actions.at(name)->state;
		}
		catch(const std::out_of_range)
		{
			std::string nameInput = name;
			LERROR("Key \"" + nameInput + "\" not found");
			return InputState::NONE;
		}
	}

	inline bool InputManager::IsPressed(const char* name)
	{
		try
		{
			return m_pInputLayout->actions.at(name)->state == InputState::PRESSED;
		}
		catch (const std::out_of_range)
		{
			std::string nameInput = name;
			LERROR("Key \"" + nameInput + "\" not found");
			return false;
		}
	}

	inline bool InputManager::IsContinue(const char* name)
	{
		try
		{
			return m_pInputLayout->actions.at(name)->state == InputState::CONTINUE;
		}
		catch (const std::out_of_range)
		{
			std::string nameInput = name;
			LERROR("Key \"" + nameInput + "\" not found");
			return false;
		}
	}

	inline bool InputManager::IsReleased(const char* name)
	{
		try
		{
			return m_pInputLayout->actions.at(name)->state == InputState::RELEASED;
		}
		catch (const std::out_of_range)
		{
			std::string nameInput = name;
			LERROR("Key \"" + nameInput + "\" not found");
			return false;
		}
	}
}