#include <Editor/InspectorWindow.h>
#include <Physics/Collider.h>
#include <Physics/RigidBody.h>
#include <Generated/Transform.rfkh.h>
#include <Core/DirLight.h>
#include <Core/PointLight.h>
#include <Core/SpotLight.h>
#include <Resources/ResourceManager.h>
#include <Resources/Scripting.h>
#include <Scripts/Gameplay.h>
#include <Sound/SoundListner.h>
#include <Sound/SoundEvent.h>
#include <UI/Image.h>
#include <ToolBox.h>
#include <UI/Text.h>

EditorLich::InspectorWindow::InspectorWindow(AssetResources* assetResource)
{
	m_pAssetResources = assetResource;
}

EditorLich::InspectorWindow::~InspectorWindow()
{
	m_pObjSelected = nullptr;
	m_pTransform = nullptr;
}

bool IsFieldValid(const rfk::Field& value,void* data) { return true; }

void EditorLich::InspectorWindow::Update()
{

	rfk::Type const& intRefType = rfk::getType<const char*>();
	std::string string = intRefType.getArchetype()->getName();

	CheckItemSelected();
	ConvertItemSelectedIntoData();

	if (EditorUI::CreateWindowUI(m_windowName,&m_isOpen,m_windowFlags))
	{
		DrawGameObject();
		DrawMaterial();
		DrawSkybox();

		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}
	else
	{
		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}



}

void EditorLich::InspectorWindow::AddInteractableWindow(IEditorWindowInterraction* window)
{
	m_interactableWindows.push_back(window);
}

void EditorLich::InspectorWindow::CheckItemSelected()
{
	ObjectType type;

	for (size_t i = 0; i < m_interactableWindows.size(); i++)
	{
		if (!m_interactableWindows[i]->HasItemSelected())  continue;

		void* dataInstance = nullptr;
		dataInstance = m_interactableWindows[i]->GetItemSelected(type);

		if (dataInstance == nullptr)
		{
			m_pObjectData = nullptr;
			m_objectType = ObjectType::NONE;
			m_interactableWindows[i]->CleanItem();
			continue;
		}

		m_interactableWindows[i]->CleanItem();
		m_pObjectData = dataInstance;
		m_objectType = type;
		break;
	}

}

void EditorLich::InspectorWindow::ConvertItemSelectedIntoData()
{
	if (m_pObjectData == nullptr) return;

	if (m_objectType == ObjectType::GAMEOBJECT)
	{
		m_pObjSelected = (SceneManagement::GameObject*)m_pObjectData;
		m_pTransform = m_pObjSelected->GetTransform();
		m_pObjectData = nullptr;
	}

	if (m_objectType == ObjectType::MATERIAL)
	{
		m_pMatSelected = (Resources::Material*)m_pObjectData;
		m_pObjectData = nullptr;
	}
	if (m_objectType == ObjectType::SKYBOX)
	{
		m_pSkyboxSelected = (CameraRender::SkyBox*)m_pObjectData;
		m_pObjectData = nullptr;
	}

}

EditorLich::FieldInfo EditorLich::InspectorWindow::GetFieldInfo(const rfk::Field* field)
{
	EditorLich::FieldInfo info;
	info.varName = field->getName();
	const rfk::Class* classTest = rfk::getDatabase().getClassById(field->getId());
	info.archetype = field->getType().getArchetype();

	if (IsArrayField(field))
	{
		info.type = rfk::EEntityKind::Class;
	}
	else
	{
		info.type = field->getType().getArchetype()->getKind();
	}
	CleanVariableName(info.varName);
	info.nameClass = field->getType().getArchetype()->getName();

	return info;
}

void EditorLich::InspectorWindow::DrawComponents()
{
	std::vector<SceneManagement::Component*>* components = (m_pObjSelected->GetComponents());
	for (size_t i = 0; i < (*components).size(); i++)
	{
		EditorUI::VerticalIndent(20);
		bool state = ComponentCheckbox(&(*components)[i]->isEnable, i);

		switch ((*components)[i]->componentType)
		{
		case SceneManagement::ComponentType::CAMERA:
		{
			if (EditorUI::CreateTreeNode("Camera",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				ResourcePopup();
				CameraRender::Camera* instance = (CameraRender::Camera*)(*components)[i];
				DrawComponent<CameraRender::Camera>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::MESH:
		{
			if (EditorUI::CreateTreeNode("Mesh",m_nodeFlags))
			{

				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				Renderer::Mesh* instance = (Renderer::Mesh*)(*components)[i];
				DrawComponent<Renderer::Mesh>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::RIGIDBODY:
		{
			if (EditorUI::CreateTreeNode("Rigidbody",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				Physics::RigidBody* instance = (Physics::RigidBody*)(*components)[i];
				DrawComponent<Physics::RigidBody>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::COLLIDER:
		{
			if (EditorUI::CreateTreeNode("Collider",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				Physics::Collider* instance = (Physics::Collider*)(*components)[i];
				DrawComponent<Physics::Collider>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::POINT_LIGHT:

		{
			if (EditorUI::CreateTreeNode("Point Light",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				ResourcePopup();
				LightSystem::PointLight* instance = (LightSystem::PointLight*)(*components)[i];
				DrawComponent<LightSystem::PointLight>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::DIR_LIGHT:
		{
			if (EditorUI::CreateTreeNode("Directionnal Light",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				LightSystem::DirLight* instance = (LightSystem::DirLight*)(*components)[i];
				DrawComponent<LightSystem::DirLight>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::SPOT_LIGHT:
		{
			if (EditorUI::CreateTreeNode("Spot Light",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				LightSystem::SpotLight* instance = (LightSystem::SpotLight*)(*components)[i];
				DrawComponent<LightSystem::SpotLight>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;

		case SceneManagement::ComponentType::ANIMATOR:
		{
			if (EditorUI::CreateTreeNode("Animator",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				AnimationSystem::Animator* instance = (AnimationSystem::Animator*)(*components)[i];
				DrawComponent<AnimationSystem::Animator>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::SCRIPTING:
		{
			if (EditorUI::CreateTreeNode("Scripting",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;


				if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::RightButton))
				{
					EditorUI::CallPopup("ComponentOptions");
				}
				ComponentPopup();

				Resources::Scripting* instance = (Resources::Scripting*)(*components)[i];
				EditorUI::TextField("Script :",instance->nameInput,200);

				if (EditorUI::Button("Validate"))
				{

					ChangeScriptingClass(instance,components,i);
					EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
					continue;

				}
				if (m_wrongScriptingName)
				{
					EditorUI::SameLine();
					EditorUI::WriteColorText(m_errorMsg[m_errorIndex],Vector4D(255,0,0,1));
				}

				const rfk::Class* test;
				if (instance->currentName == "Scripting")
				{
					test = rfk::getDatabase().getNamespaceByName("Resources")->getClassByName("Scripting");

				}
				else
				{
					test = rfk::getDatabase().getFileLevelClassByName(instance->currentName.c_str());
				}
				//ResourcePopup();
				DrawComponent(test,(*components)[i]);

				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::SOUNDLISTNER:
		{
			if (EditorUI::CreateTreeNode("Sound Listner",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				SoundSystem::SoundListner* instance = (SoundSystem::SoundListner*)(*components)[i];
				DrawComponent<SoundSystem::SoundListner>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::SOUNDEVENT:
		{
			if (EditorUI::CreateTreeNode("Sound Event",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				SoundSystem::SoundEvent* instance = (SoundSystem::SoundEvent*)(*components)[i];
				DrawComponent<SoundSystem::SoundEvent>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::IMAGE:
		{
			if (EditorUI::CreateTreeNode("Image UI",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				//ResourcePopup();
				UI::Image* instance = (UI::Image*)(*components)[i];
				DrawComponent<UI::Image>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
			}
		}
		break;
		case SceneManagement::ComponentType::TEXT:
		{
			if (EditorUI::CreateTreeNode("Text UI",m_nodeFlags))
			{
				m_typeComponentSelected = (*components)[i]->componentType;
				
				UI::Text* instance = (UI::Text*)(*components)[i];
				DrawComponent<	UI::Text>((*components)[i]);
				EditorUI::End(EditorUI::TypeUI::TreeNode ,__FILE__, __LINE__);
			}
		}
		break;

		default:
			continue;
			break;
		}

		if (state)
			(*components)[i]->ChangeEnableState();
	}

}

bool EditorLich::InspectorWindow::ComponentCheckbox(bool* ptrState, int seed)
{
	EditorUI::ColorStyle style = EditorUI::ColorObject::CheckBackground;
	EditorUI::ColorStyle style2 = EditorUI::ColorObject::FrameBgHovered;
	EditorUI::PushStyle(EditorUI::ColorObject::FrameBgHovered,Vector4D(0.4f,0.4f,0.4f,1));
	EditorUI::PushStyle(EditorUI::ColorObject::FrameBgActive,Vector4D(0.4f,0.4f,0.4f,1));
	EditorUI::PushStyle(EditorUI::ColorObject::Border,Vector4D(0.6f,0.6f,0.6f,1));
	EditorUI::PushStyle(style,Vector4D(0.2f,0.2f,0.2f,1));
	EditorUI::PushStyle(EditorUI::VariableStyleFlags::FrameBorderSize,1);

	std::string name = "ActiveComponent" + std::to_string(seed);
	bool state = EditorUI::Checkbox(ptrState, name.c_str());

	EditorUI::PopStyle(EditorUI::StyleType::Var);
	EditorUI::PopStyle(EditorUI::StyleType::Color);
	EditorUI::PopStyle(EditorUI::StyleType::Color);
	EditorUI::PopStyle(EditorUI::StyleType::Color);
	EditorUI::PopStyle(EditorUI::StyleType::Color);
	EditorUI::SameLine();
	return state;
}

void EditorLich::InspectorWindow::DrawField(const rfk::Field* field,bool& asBeenModified,void* dataInstance)
{

	FieldInfo info = GetFieldInfo(field);
	const TitleReflectionProperty* propertyField = field->getProperty<TitleReflectionProperty>(0);
	if (propertyField)
	{
		EditorUI::WriteText(propertyField->GetTitle());
	}

	const SameLineProperty* propertyField2 = field->getProperty<SameLineProperty>(0);
	if (propertyField2)
	{
		EditorUI::SameLine();
	}

	DrawFieldFundamental(field,field->getPtrUnsafe(dataInstance),info.changeField,info.asBeenHandle);
	DrawEnum(field,dataInstance,info);
	DrawClassField(field,dataInstance,info);

	if (!asBeenModified) asBeenModified = info.changeField;
}

void EditorLich::InspectorWindow::DrawFields(rfk::Vector<const rfk::Field*> fields,void* data,bool& asChange)
{
	for (size_t i = 0; i < fields.size(); i++)
	{
		DrawField(fields[i],asChange,data);
	};
}

bool EditorLich::InspectorWindow::IsArrayField(const rfk::Field* field)
{
	rfk::ClassTemplateInstantiation const* classTemplateInstance = rfk::classTemplateInstantiationCast(field->getType().getArchetype());

	if (classTemplateInstance == nullptr) return false;

	if (classTemplateInstance->getClassTemplate() != *rfk::getDatabase().getFileLevelClassByName("VectorContainer")) 	return false;

	return true;
}

void EditorLich::InspectorWindow::DrawArray(const rfk::Field* field,void* instance,bool& asChange,const char* nameVar)
{
	void* arrayData = field->getPtrUnsafe(instance);
	const rfk::Class* arrayClass = rfk::classCast(field->getType().getArchetype());
	const rfk::Method* classFunc = arrayClass->getMethodByName("GetClassArchetype");
	const rfk::Method* TestFundamentalFunc = arrayClass->getMethodByName("IsFundamentalData");
	const rfk::Method* TestPointerFunc = arrayClass->getMethodByName("IsPointer");
	const rfk::Method* dataFunc = arrayClass->getMethodByName("GetDataIndexPointer");
	const rfk::Method* countFunc = arrayClass->getMethodByName("GetSize");
	const rfk::Method* addFunc = arrayClass->getMethodByName("AddObject");
	const rfk::Method* removeFunc = arrayClass->getMethodByName("RemoveObject");

	unsigned int count = countFunc->invokeUnsafe<int>(arrayData);

	if (TestFundamentalFunc->invokeUnsafe<bool>(arrayData))
	{
		const rfk::Archetype* archeType = classFunc->invokeUnsafe<const rfk::Archetype*>(instance);
		std::string type = archeType->getName();
		EditorUI::WriteText(nameVar);
		EditorUI::Indent();

		for (unsigned int i = 0; i < count; i++)
		{
			const rfk::Archetype* archeType = classFunc->invokeUnsafe<const rfk::Archetype*>(instance);
			void* data = dataFunc->invokeUnsafe<void*,int>(arrayData,i);
			std::string name = "Element " + std::to_string(i);
			bool isHandle = false;
			DrawPrimitive(type.c_str(),name.c_str(),data,asChange,archeType,field, isHandle);
		}

		EditorUI::Unindent();

	}
	if (EditorUI::Button("+",field->getName()))
	{
		addFunc->invokeUnsafe<void>(arrayData);
	}
	EditorUI::SameLine();
	if (EditorUI::Button("-"))
	{
		removeFunc->invokeUnsafe<void>(arrayData);
	}
}

void EditorLich::InspectorWindow::DrawMaterial()
{
	if (m_objectType != ObjectType::MATERIAL) return;

	EditorUI::WriteText(m_pMatSelected->name.data());

	bool asChange = false;
	const rfk::Archetype* archetype = rfk::getArchetype<Resources::Material>();
	const rfk::Class* classInstance = rfk::classCast(archetype);
	rfk::Vector<const rfk::Field*> fields = GetFields((void*)m_pMatSelected, classInstance);

	ResourcePopup();
	DrawFields(fields, (void*)m_pMatSelected, asChange);
	EditorUI::VerticalIndent();
	EditorUI::CenterObject("Save");
	if (EditorUI::Button("Save"))
	{
		m_pMatSelected->SaveInMatFile();
	}

}
void EditorLich::InspectorWindow::DrawSkybox()
{

	if (m_objectType != ObjectType::SKYBOX) return;

	EditorUI::WriteText(m_pSkyboxSelected->name.data());

	bool asChange = false;
	const rfk::Archetype* archetype = rfk::getArchetype<CameraRender::SkyBox>();
	const rfk::Class* classInstance = rfk::classCast(archetype);
	rfk::Vector<const rfk::Field*> fields = GetFields((void*)m_pSkyboxSelected, classInstance);

	ResourcePopup();
	DrawFields(fields, (void*)m_pSkyboxSelected, asChange);
	
	EditorUI::VerticalIndent();
	EditorUI::CenterObject("Save");
	if (EditorUI::Button("Save"))
	{
		m_pSkyboxSelected->SaveInSkyboxFile();
	}
}

void EditorLich::InspectorWindow::DrawGameObject()
{
	if (m_objectType != ObjectType::GAMEOBJECT || m_pObjSelected == nullptr) return;

	GameObjectName();

	if (EditorUI::CreateTreeNode("Transform",m_nodeFlags))
	{
		DrawComponent<Physics::Transform>((SceneManagement::Component*)m_pTransform);
		EditorUI::End(EditorUI::TypeUI::TreeNode ,__FILE__, __LINE__);
	}

	DrawComponents();
	EditorUI::CenterObject("Add Component");
	if (EditorUI::Button("Add Component"))
	{
		EditorUI::CallPopup("Component Popup");
	}

	if (EditorUI::CreatePopup("Component Popup"))
	{
		const rfk::Enum* enumInstance = rfk::getEnum<SceneManagement::ComponentType>();

		const uint size = enumInstance->getEnumValuesCount();

		std::vector<const char*> arrayName;

		for (uint i = 0; i < size; i++)
		{
			arrayName.push_back(enumInstance->getEnumValueAt(i).getName());
		}
		if (EditorUI::CreateComboField("Component",m_indexComponentAdd,arrayName.data(),size))
		{
			m_pObjSelected->AddSpecificComponent((SceneManagement::ComponentType)m_indexComponentAdd, false);
			EditorUI::Close(EditorUI::TypeUI::Popup);
		}
		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}


}

void EditorLich::InspectorWindow::GameObjectName()
{
	bool* state = m_pObjSelected->GetState();
	if (EditorUI::Checkbox(state, "EnableGameObject"))
	{
		m_pObjSelected->UpdateState();
	}
	EditorUI::SameLine();


	char* text = m_pObjSelected->GetNamePtr()->data();

	EditorUI::TextField("", text, 100);
	(*m_pObjSelected->GetNamePtr()) = text;
	if (!EditorUI::ItemAction(EditorUI::ActionType::Active))
	{
		if (m_pObjSelected->GetNamePtr()->c_str()[0] == '\0')
			m_pObjSelected->RestoreName();

		m_pObjSelected->prevName = m_pObjSelected->GetNamePtr()->c_str();
	}
	EditorUI::VerticalIndent();
}

void EditorLich::InspectorWindow::DrawFieldFundamental(const rfk::Field* field,void* data,bool& asChange, bool& ishandle)
{
	const rfk::Archetype* fieldVarArchectype = field->getType().getArchetype();
	const char* type = fieldVarArchectype->getName();
	std::string name = field->getName();
	CleanVariableName(name);
	DrawPrimitive(type,name,data,asChange,fieldVarArchectype,field, ishandle);
}

void EditorLich::InspectorWindow::DrawEnum(const rfk::Field* field,void* data,FieldInfo info)
{
	if (info.type == rfk::EEntityKind::Enum)
	{
		const rfk::Archetype* archetype = field->getType().getArchetype();
		const rfk::Enum* enumInstance = rfk::enumCast(archetype);
		const uint size = enumInstance->getEnumValuesCount();

		std::vector<const char*> arrayName;
		int* state = (int*)(field->getPtrUnsafe(data));

		for (uint i = 0; i < size; i++)
		{
			arrayName.push_back(enumInstance->getEnumValueAt(i).getName());
		}
		info.changeField = EditorUI::CreateComboField(info.varName.c_str(),*state,arrayName.data(),size);
		info.asBeenHandle = true;
	}
}

rfk::Vector<const rfk::Field*> EditorLich::InspectorWindow::GetFields(void* data,const rfk::Class* classIntance)
{
	rfk::Predicate<rfk::Field> fieldTest = IsFieldValid;
	return classIntance->getFieldsByPredicate(fieldTest,data,true,true);
}

void EditorLich::InspectorWindow::CleanVariableName(std::string& varName)
{
	if (varName[1] == '_' && varName[2] == 'p' && std::isupper(varName[3]))
	{
		varName = varName.substr(3,varName.size() - 3);
	}
	if (varName[1] == '_')
	{
		varName = varName.substr(2,varName.size() - 2);
	}
	varName[0] = std::toupper(varName[0]);
	for (size_t i = 1; i < varName.size(); i++)
	{
		if (std::isupper(varName[i]) )
		{
			varName.insert(i, " ");
			i++;
		}
	}


}

void EditorLich::InspectorWindow::DrawClassField(const rfk::Field* field,void* data,FieldInfo info)
{
	rfk::Predicate<rfk::Field> fieldTest = IsFieldValid;
	const rfk::Archetype* archetype = field->getType().getArchetype();
	if (info.asBeenHandle) return;

	rfk::Vector<const rfk::Field*> fieldsArray;

	if (info.type == rfk::EEntityKind::Class)
	{
		if (IsArrayField(field))
		{
			DrawArray(field,data,info.asBeenHandle,info.varName.c_str());
			return;
		}
		const rfk::Class* classInstance = rfk::classCast(archetype);
		fieldsArray = classInstance->getFieldsByPredicate(fieldTest,data);

	}
	if (info.type == rfk::EEntityKind::Struct)
	{
		const rfk::Struct* structInstance = rfk::structCast(archetype);
		fieldsArray = structInstance->getFieldsByPredicate(fieldTest,data);
	}
	DrawFields(fieldsArray,(void*)field->getPtrUnsafe(data),info.changeField);
	info.asBeenHandle = true;
}

void EditorLich::InspectorWindow::DrawPrimitive(std::string type,std::string name,void* data,bool& asChange,const rfk::Archetype* archetype,const rfk::Field* field, bool& isHandle)
{
	if (type == "Vector2D")
	{
		float min = 0;
		float max = 0;
		float sensibility = 1;
		const RangeProperty* propertyField = field->getProperty<RangeProperty>(0);
		if (propertyField)
		{
			min = propertyField->GetMin();
			max = propertyField->GetMax();
			if ((max - min) / 10 < 1) 
				sensibility = (max - min) / 10.f;
		}
		Vector2D* instance = (Vector2D*)data;
		asChange = EditorUI::SliderVec2(instance,name.c_str(),sensibility,min,max);
		isHandle = true;
	}

	if (type == "Vector3D")
	{
		Vector3D* instance = (Vector3D*)data;
		asChange = EditorUI::SliderVec3(instance,name.c_str());
		isHandle = true;
	}
	if (type == "Vector4D")
	{
		Vector4D* instance = (Vector4D*)data;
		asChange = EditorUI::ColorField(instance,name.c_str());
		isHandle = true;
	}

	if (type == "float")
	{
		float* instance = (float*)data;
		asChange = EditorUI::FloatField(instance,name.c_str());
		isHandle = true;
	}

	if (type == "int")
	{
		int* instance = (int*)data;
		asChange = EditorUI::IntField(instance,name.c_str());
		isHandle = true;

	}
	if (type == "bool")
	{
		bool* instance = (bool*)data;
		asChange = EditorUI::BooleanField(instance,name.c_str());
		isHandle = true;

	}
	if (type == "char")
	{
		const char** instance = (const char**)data;
		char* text = (char*)(*instance);
		asChange = EditorUI::TextField(name.c_str(),text,100);
		instance = (const char**)(&text);
		isHandle = true;
	}

	const rfk::Class* classInstance = rfk::classCast(archetype);
	if (classInstance->isSubclassOf(Resources::Resource::staticGetArchetype()))
	{
		Resources::Resource** ptrInstance = (Resources::Resource**)data;
		Resources::Resource* instance = *ptrInstance;
		const ResourceTypeProperty* propertyField = field->getProperty<ResourceTypeProperty>(0);
	
		std::string nameResource = "None";
		if (instance != nullptr) nameResource = instance->name;

		std::string nameVar = name + ':';
		if (EditorUI::SelectableField(nameVar.c_str(),nameResource.c_str(),EditorUI::MouseButtonUI::RightButton))
		{
			if (propertyField != nullptr)
				m_typeSelected = propertyField->GetType();
			m_pSelected = ptrInstance;
			EditorUI::CallPopup("Resource");
		}
		isHandle = true;
	}



}

bool EditorLich::InspectorWindow::ResourcePopup()
{
	bool change = false;
	if (EditorUI::CreatePopup("Resource"))
	{
		std::vector<EditorLich::AssetData*> dataAssets;
		if ((*m_pSelected) == nullptr)
			dataAssets = m_pAssetResources->GetAssetType(m_typeSelected);
		else
		{
			dataAssets = m_pAssetResources->GetAssetType((*m_pSelected)->type);
			m_typeSelected = (*m_pSelected)->type;
		}
		if (EditorUI::SelectableField("","None",EditorUI::MouseButtonUI::LeftButton))
		{
			(*m_pSelected) = nullptr;
			change = true;
		}
		for (size_t i = 0; i < dataAssets.size(); i++)
		{
			if (EditorUI::SelectableField("",dataAssets[i]->name.c_str(),EditorUI::MouseButtonUI::LeftButton))
			{
				char* namePtr = new char[dataAssets[i]->name.size()+1];
				strcpy_s(namePtr, dataAssets[i]->name.size()+1, dataAssets[i]->name.c_str()); 
				Utils::remove_extensionChr(namePtr);
				Resources::Create(m_typeSelected,dataAssets[i]->path.string(), namePtr);
				(*m_pSelected) = Resources::ResourceManager::GetSpecificResource(m_typeSelected, namePtr);
				change = true;
			}
		}
		if (dataAssets.size() == 0)
		{
			EditorUI::WriteText("No resources");
		}
		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}
	return change;
}



void  EditorLich::InspectorWindow::DrawComponent(const rfk::Class* classInstance,SceneManagement::Component* instance)
{

	bool asChange = false;

	rfk::Vector<const rfk::Field*> fields = GetFields((void*)instance,classInstance);

	DrawFields(fields,(void*)instance,asChange);

	if (!asChange) return;
	instance->ApplyEditorChange();
}

void EditorLich::InspectorWindow::ComponentPopup()
{
	if (EditorUI::CreatePopup("ComponentOptions"))
	{
		if (EditorUI::Button("Remove Component"))
		{
			m_pObjSelected->RemoveSpecificComponent(m_typeComponentSelected);
		}
		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}
}

void EditorLich::InspectorWindow::ChangeScriptingClass(Resources::Scripting* script,std::vector<SceneManagement::Component*>* components,int index)
{
	SceneManagement::GameObject* GO = script->gameObject;
	Core::InputManager* input = script->input;


	std::string name;
	name.reserve(100);
	name = script->nameInput;
	const rfk::Class* scriptClass = rfk::getDatabase().getFileLevelClassByName(script->nameInput.c_str());

	if (IsWrongScriptName(scriptClass) || IsAlreadySet(script->currentName,name)) return;

	m_wrongScriptingName = false;

	rfk::UniquePtr<Resources::Scripting> ptr = scriptClass->makeUniqueInstance<Resources::Scripting>();
	script = (Resources::Scripting*)ptr.release();
	script->currentName = name;
	script->nameInput = name;

	script->gameObject = GO;
	script->input = input;
	
	(*components)[index] = script;
}

bool EditorLich::InspectorWindow::IsAlreadySet(std::string currentScriptName,std::string inputScriptName)
{
	if (currentScriptName != inputScriptName) return false;
	m_wrongScriptingName = true;
	m_errorIndex = 1;
	return true;
}

bool EditorLich::InspectorWindow::IsWrongScriptName(const rfk::Class* scriptClass)
{
	if (scriptClass != nullptr) return false;
	m_wrongScriptingName = true;
	m_errorIndex = 0;
	return true;
}





