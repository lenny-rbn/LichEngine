#pragma once


#include <Define.h>
#include <string>
#include <Generated/Component.rfkh.h>

namespace Physics
{
	class Transform;
}

namespace SceneManagement NAMESPACE()
{
	class GameObject;

	enum ENUM() ComponentType
	{
		CAMERA,
		DIR_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		MESH,
		RIGIDBODY,
		COLLIDER,
		ANIMATOR,
		SCRIPTING,
		SOUNDLISTNER,
		SOUNDEVENT,
		TEXT,
		IMAGE,
	};

	class LICH_ENGINE Component
	{
	public:
		virtual ~Component() { };
		virtual void Update(Physics::Transform& transform) = 0;
		virtual void LoadData(std::string data) = 0;
		virtual std::string SaveData() = 0;
		virtual void ApplyEditorChange() = 0;


		bool isEnable = true;
		ComponentType componentType;
		bool IsEnableChange() { return p_enableTransition; }
		void ChangeEnableState();
		void ResetEnableTransition() { p_enableTransition = false; };
		GameObject* GetGameObject() { return p_pGameObject; };
	protected :
		bool p_enableTransition = false;
		GameObject* p_pGameObject = nullptr;
	private : 
		bool CheckEnableState() { return (isEnable == m_prevEnableState); }
		bool m_prevEnableState = true;
	};
}
File_Component_GENERATED