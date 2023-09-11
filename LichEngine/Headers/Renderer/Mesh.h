#pragma once
#include <Resources/Model.h>
#include <Resources/Shader.h>
#include <Scene/Component.h>
#include <ToolBox.h>
#include <Resources/ResourcerReflectionProperty.h>
#include <Generated/Mesh.rfkh.h>


namespace Renderer NAMESPACE()
{
	class CLASS() LICH_ENGINE Mesh : public SceneManagement::Component
	{
	public:
		Mesh(SceneManagement::GameObject * gO);
		Mesh(SceneManagement::GameObject * gO, Resources::Model * modelGive);
		~Mesh();

		void SetModel(Resources::Model * model);
		bool HasMaterialsGeneratingShadow();
		bool HasTransparentMaterials();

		void Update(Physics::Transform & transform) override;
		void LoadData(std::string data) override;
		void ApplyEditorChange() override;
		std::string SaveData() override;

		inline Resources::Model* GetModel() {return m_pModel; }
		inline VectorContainer<Resources::Material> GetMaterials() { return m_pMaterial; }
		Resources::Material* GetMaterial(int index);

		inline bool IsForwardRendering() { return m_forwardRendering; }

		inline void SetDrawForward(bool forward) { m_forwardRendering = forward; }

	private:
		bool m_materialSet;
		bool m_forwardRendering;
		bool m_isEmpty = false;

		FIELD(ResourceTypeProperty(Resources::ResourceType::MODEL)) Resources::Model* m_pModel;
		FIELD(ResourceTypeProperty(Resources::ResourceType::MATERIAL)) VectorContainer<Resources::Material> m_pMaterial;

		Renderer_Mesh_GENERATED
	};
};
File_Mesh_GENERATED