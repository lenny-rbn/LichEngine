#pragma once
#include <vector>

#include <Maths.h>
#include <Scene/Component.h>
#include <Generated/Transform.rfkh.h>

namespace SceneManagement
{
	class GameObject;
}

namespace CameraRender
{
	class Camera;
}

namespace Physics NAMESPACE()
{
	enum class LocalModification
	{
		SET_POSITION,
		SET_ROTATION_EULER,
		SET_ROTATION_QUATERNION,
		SET_SCALE,
	};

	enum class GlobalModification
	{
		SET_POSITION,
		SET_ROTATION_EULER,
		SET_ROTATION_QUATERNION,
		SET_SCALE,
		TRANSLATE,
		ROTATE_EULER,
		ROTATE_QUATERNION,
		SCALE,
	};

	class CLASS() LICH_ENGINE  Transform : public SceneManagement::Component
	{
	private:
		FIELD() Vector3D   m_localPosition;
		FIELD() Vector3D   m_localRotation; //Tait-Bryan angles : roll, pitch, yaw
		FIELD() Vector3D   m_localScale;
		Quaternion m_localRot;

		Vector3D   m_globalPosition;
		Vector3D   m_globalRotation; //Tait-Bryan angles : roll, pitch, yaw
		Vector3D   m_globalScale;
		Quaternion m_globalRot;
				   
		Vector3D   m_up;
		Vector3D   m_right;
		Vector3D   m_forward;
				   
		Matrix4D   m_localModelMatrix;
		Matrix4D   m_globalModelMatrix;
		Matrix4D   m_mvp;

		bool m_localsHasBeenModified  = false;
		bool m_globalsHasBeenModified = true;
		bool m_modifiedByPhysics = false;
		bool m_postPhysicUpdated = false;
		bool m_updated = false;

		Transform* m_pParent = nullptr;
		std::vector<Transform*> m_children;
		SceneManagement::GameObject* m_pCurrentObject;

		bool m_isPlaying;

		int m_sceneIndex = 0;

		std::vector<std::pair<LocalModification, Vector4D>> m_localModifications;
		std::vector<std::pair<GlobalModification, Vector4D>> m_globalModifications;

		void CalculateModelMatrix();
		void CalculateMVP(Matrix4D vp);
		void UpdateAllGlobalVariables();
		void UpdateAllLocalVariables();
		void ApplyLocalModifications();
		void ApplyGlobalModifications();
		bool VerifyChild(Transform* child);
		bool VerifyChild(Transform* parent,Transform* child);

	public:
		Transform();
		Transform(SceneManagement::GameObject* obj);
		Transform(SceneManagement::GameObject* obj,Vector3D pos);
		Transform(SceneManagement::GameObject* obj,Vector3D pos,Quaternion rot);
		Transform(SceneManagement::GameObject* obj,Vector3D pos, Vector3D rot);
		Transform(SceneManagement::GameObject* obj,Vector3D pos, Vector3D rot, Vector3D sca);
		~Transform();
		
		void CalculateTemp();
		void PostPhysicUpdate();
		void Update(Physics::Transform& transform)  override;
		void Update(CameraRender::Camera* cam, bool isParentUpdated, bool isPlaying) ;
		void UpdateFromPhysics(const Transform& transform);

		Transform UpdatePhysics();
		std::string SaveData() override;
		void LoadData(std::string content) override;
		void ApplyEditorChange()  override;

		void SetGlobalPosition(const Vector3D& pos);
		void SetGlobalRotation(const Vector3D& rot);
		void SetGlobalRotation(const Quaternion& rot);
		void SetGlobalScale(const Vector3D& sca);

		void SetLocalPosition(const Vector3D& pos);
		void SetLocalRotation(const Vector3D& rot);
		void SetLocalRotation(const Quaternion& rot);
		void SetLocalScale(const Vector3D& sca);

		Vector3D GetGlobalPosition()					{ return m_globalPosition; }
		Vector3D GetGlobalRotation()					{ return m_globalRotation; }
		Quaternion GetGlobalRotationInQuaternion()		{ return m_globalRot; }
		Vector3D GetGlobalScale()						{ return m_globalScale; }
																 
		Vector3D GetLocalPosition()						{ return m_localPosition; }
		Vector3D GetLocalRotation()						{ return m_localRotation; }
		Quaternion GetLocalRotationInQuaternion()		{ return m_localRot; }
		Vector3D GetLocalScale()						{ return m_localScale; }

		Vector3D GetVectorUp();
		Vector3D GetVectorRight();
		Vector3D GetVectorForward();

		Matrix4D GetModelMatrice()						{ return m_globalModelMatrix; }
		Matrix4D GetMVP()								{ return m_mvp; }
		int GetSceneIndex()								{ return m_sceneIndex; }
		void SetSceneIndex(int index)					{ m_sceneIndex = index; }

		void Translate(const Vector3D& translation);
		void Rotate(const Vector3D& eulerRotation);
		void Rotate(const Quaternion& quaternionRotation);
		void Scale(const Vector3D& scaling);

		bool HasParent()								{ return m_pParent != nullptr; }
		Transform* GetParent()							{ return m_pParent; }
		std::vector<Transform*> GetChildrenArray()		{ return m_children; }
		std::vector<Transform*>* GetChildrenArrayPtr()	{ return &m_children; }
		SceneManagement::GameObject* GetGameObject() 	{ return m_pCurrentObject; }

		void RemoveParent(bool isStart);
		void AddChild(Transform* transform);
		void AddParent(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChilds();
		
		void ResetUpdateState()							{ m_updated = false; }

		bool IsPlaying() { return m_isPlaying; }

		Physics_Transform_GENERATED
	};
};
File_Transform_GENERATED