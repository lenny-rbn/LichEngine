#include <Scene/GameObject.h>
#include <Debugs/Log.h>
#include <Scene/SceneGraph.h>
#include <Scene/Scene.h>
#include <Editor/FileBrowserWindow.h>
#include <Editor/HierachyWindow.h>

EditorLich::HierachyWindow::HierachyWindow(SceneManagement::SceneManager* sceneManager, EditorLich::FileBrowser* filebrowser)
{
	m_pSceneManager = sceneManager;
	m_pFilebrowser = filebrowser;
	input = sceneManager->GetInputManager();
	m_windowFlags = EditorUI::WindowOptions::MenuBar;
}

EditorLich::HierachyWindow::~HierachyWindow()
{
}

void EditorLich::HierachyWindow::Update()
{
	if (EditorUI::CreateUI(EditorUI::TypeUI::Window, m_windowName, &m_isOpen, m_windowFlags))
	{
		if (EditorUI::CreateUI(EditorUI::TypeUI::MenuBar))
		{	
			if (EditorUI::CreateUI(EditorUI::TypeUI::Menu, "+"))
			{
				if (ImGui::MenuItem("Create GameObject"))
				{
					m_mousePos = EditorUI::GetMousePos();
					m_openFieldPopup = true;
				}
				if (m_gameObjectSelected && ImGui::MenuItem("Remove GameObject"))
				{
					m_pSceneGraph->RemoveGameObject(m_gameObjectSelected);
					m_hasItemSelected = true;
					m_pData = nullptr;
					m_type = ObjectType::NONE;
					m_pSceneGraph->GetScene()->GetGizmos()->SetGO(nullptr);
				}

				EditorUI::End(EditorUI::TypeUI::Menu, __FILE__, __LINE__);
			}


			EditorUI::End(EditorUI::TypeUI::MenuBar, __FILE__, __LINE__);
		}

		// Duplication of a gameObject
		if (!m_pSceneManager->IsCurrentSceneIsPlaying() && m_gameObjectSelected != nullptr)
		{
			if (input->IsContinue("Ctrl") && input->IsPressed("D"))
			{
				m_pSceneGraph->DuplicateGameObject(m_gameObjectSelected);
			}
		}


		// Draw Scene Object
		ManageSceneState();
		m_pSceneGraph = (*m_pScene)->GetSceneGraph();
		SceneNameField();

		WriteObjectName();
		DeselectingObject();

		HierachyMenu();
		DrawGameObjectNamePopup();

		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}
	else
	{
		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}
}

void* EditorLich::HierachyWindow::GetItemSelected(ObjectType& type)
{
	type = m_type;
	return m_pData;
}

void EditorLich::HierachyWindow::SetItemSelected(SceneManagement::GameObject* gO)
{
	m_hasItemSelected = true;
	m_pData = gO;
	m_gameObjectSelected = gO;
	m_type = ObjectType::GAMEOBJECT;
	m_pSceneGraph->GetScene()->GetGizmos()->SetGO(gO);
}

void EditorLich::HierachyWindow::SetCurrentSceneGraph(SceneManagement::Scene** scene)
{
	m_pScene = scene;
}

void EditorLich::HierachyWindow::WriteObjectName()
{
	std::vector<Physics::Transform*>* ptrArray = (m_pSceneGraph->GetTransformArray());
	std::vector<Physics::Transform*> objectArray = (*ptrArray);

	int nextPos = m_currentIndex + m_hierachyMouvement;
	nextPos = Clamp(nextPos, 0, (*ptrArray).size() - 1);
	m_nextPos = nextPos;

	for (uint i = 0; i < objectArray.size(); i++)
	{
		if (nextPos == i && m_hasDragDrop)
		{
			EditorUI::VerticalIndent(m_verticalIndentOfDropMenu);
			//EditorUI::Separator();
		}
		/*if (nextPos == i && m_childAction == 2 && m_hasDragDrop)
		{
			EditorUI::Indent();

		}*/
		objectArray[i]->SetSceneIndex(i);

		if (objectArray[i]->HasParent()) continue;


		WriteObject(objectArray[i], i);
		if (nextPos == i && m_childAction == 2 && m_hasDragDrop)
		{
			EditorUI::Separator();
		}

		if (nextPos == i && m_childAction == 2 && m_hasDragDrop) EditorUI::Unindent();
	}
}

void EditorLich::HierachyWindow::WriteObject(Physics::Transform* transform, int index)
{

	std::string name = (transform->GetGameObject()->GetName());

	std::vector<Physics::Transform*> childArray = (*transform->GetChildrenArrayPtr());
	bool node_open = false;


	if (childArray.size() == 0)
	{

		EditorUI::Tree_Flags node_flags = EditorUI::TreeOptions::Leaf | EditorUI::TreeOptions::OpenDoubleClick;
		if (EditorUI::CreateUI(EditorUI::TypeUI::TreeNode, node_flags, name.c_str()))
		{
			EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
		}
		DragAndDrop(transform);

		return;
	}
	EditorUI::Tree_Flags node_flags = EditorUI::TreeOptions::OpenArrow | EditorUI::TreeOptions::OpenDoubleClick;
	if (EditorUI::CreateUI(EditorUI::TypeUI::TreeNode, node_flags, name.c_str()))
	{
		DragAndDrop(transform);
		std::vector<Physics::Transform*>* childArray1 = (transform->GetChildrenArrayPtr());
		if (childArray.size() == 0 && !node_open) return;



		for (size_t i = 0; i < childArray.size(); i++)
		{

			index++;
			EditorUI::Indent();

			WriteObject(childArray[i], index);

			if (m_nextPos == index && m_childAction == 2 && m_hasDragDrop)
			{
				EditorUI::Separator();
			}
			EditorUI::Unindent();
		}
		EditorUI::End(EditorUI::TypeUI::TreeNode, __FILE__, __LINE__);
	}
	else
	{
		DragAndDrop(transform);
	}

}

void EditorLich::HierachyWindow::DragAndDrop(Physics::Transform* transform)
{
	HierachyActions(transform);
	RegisterHierachyInput(transform);
}

void EditorLich::HierachyWindow::HierachyActions(Physics::Transform* transform)
{
	std::vector<Physics::Transform*>* ptrArray = (m_pSceneGraph->GetTransformArray());

	if (!ImGui::IsItemActive() && m_currentIndex == transform->GetSceneIndex() && !ImGui::IsItemHovered())
	{
		int nextPos = m_currentIndex + m_hierachyMouvement;
		nextPos = Clamp(nextPos, 0, (*ptrArray).size() - 1);

		HierachyChildParentAction(ptrArray, nextPos);
		HierachySwapObject(ptrArray, nextPos);

		m_hierachyMouvement = 0;
		m_childAction = NONE;
		m_hasDragDrop = false;
		m_prevMousePos = Vector2D::zero;
		m_currentIndex = -1;
	}
}

void EditorLich::HierachyWindow::HierachySwapObject(std::vector<Physics::Transform*>* ptrArray, int nextPos)
{
	if (m_hierachyMouvement == 0) return;
	if (m_hierachyMouvement <0 && m_currentIndex == 0) return;
	Physics::Transform* currentValue = (*ptrArray)[m_currentIndex];
	(*ptrArray).erase((*ptrArray).begin() + m_currentIndex);

	
	if (!currentValue->HasParent())
	{
		if (m_childAction == NONE)
		{
			(*ptrArray).insert((*ptrArray).begin() + nextPos, currentValue);
			return;
		}
	}	
	m_pSceneGraph->CalculateSceneIndex();
		(*ptrArray).insert((*ptrArray).begin() + currentValue->GetSceneIndex(), currentValue);
}

void EditorLich::HierachyWindow::HierachyChildParentAction(std::vector<Physics::Transform*>* ptrArray, int nextPos)
{
	if (m_childAction == ADD)
	{
		if (nextPos != m_currentIndex)
			(*ptrArray)[nextPos]->AddChild((*ptrArray)[m_currentIndex]);
	}
	if (m_childAction == REMOVE)
	{

		(*ptrArray)[m_currentIndex]->RemoveParent(true);
	}
}

void EditorLich::HierachyWindow::HierachyMenu()
{
	if (EditorUI::CreatePopupWindow(m_hierachyMenuName))
	{
		if (EditorUI::Button("Add GameObject"))
		{
			m_mousePos = EditorUI::GetMousePos();
			m_openFieldPopup = true;
			EditorUI::Close(EditorUI::TypeUI::Popup);
		}

		if (EditorUI::Button("Duplicate"))
		{
			m_pSceneGraph->DuplicateGameObject(m_gameObjectSelected);
			EditorUI::Close(EditorUI::TypeUI::Popup);
		}

		if (m_pData != nullptr && EditorUI::Button("Remove GameObject"))
		{
			m_pSceneGraph->RemoveGameObject(m_gameObjectSelected);
			m_hasItemSelected = true;
			m_pData = nullptr;
			m_type = ObjectType::NONE;
			m_pSceneGraph->GetScene()->GetGizmos()->SetGO(nullptr);
			EditorUI::Close(EditorUI::TypeUI::Popup);
		}

		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}

	if (m_openFieldPopup)
	{
		EditorUI::CallPopup(m_gameObjectInputTextName);
		m_openFieldPopup = false;
	}
}

void EditorLich::HierachyWindow::DrawGameObjectNamePopup()
{
	if (EditorUI::CreatePopup(m_gameObjectInputTextName))
	{
		EditorUI::SetWindowPos(m_mousePos);

		EditorUI::WriteText("Enter Name");

		EditorUI::TextField("Name", m_nameInput, 100);
		EditorUI::SameLine();
		if (EditorUI::Button("Validate")) CreateNewGameObject();

		EditorUI::SameLine();

		if (EditorUI::Button("Cancel"))
		{
			EditorUI::Close(EditorUI::TypeUI::Popup);
			m_errorNameInput = "";
		}

		EditorUI::WriteColorText(m_errorNameInput, Vector4D(255, 0, 0, 255));

		EditorUI::End(EditorUI::TypeUI::Popup, __FILE__, __LINE__);
	}
}

void EditorLich::HierachyWindow::CreateNewGameObject()
{
	if (!m_pSceneGraph->isNameIsUse(m_nameInput.c_str()) && m_nameInput.c_str() != "")
	{
		m_pSceneGraph->AddGameObject(m_nameInput.c_str());
		EditorUI::Close(EditorUI::TypeUI::Popup);
		m_errorNameInput = "";
	}
	else
	{
		m_errorNameInput = "Name Invalid";
	}
}

void EditorLich::HierachyWindow::ManageSceneState()
{
	if ((*m_pScene)->IsPlaying())
	{
		m_isPlaying = true;
	}
	if (!(*m_pScene)->IsPlaying() && m_isPlaying)
	{
		m_hasItemSelected = true;
		m_pData = nullptr;
		m_type = ObjectType::NONE;
		m_isPlaying = false;
	}
}

void EditorLich::HierachyWindow::SceneNameField()
{
	char* nameData = (*m_pScene)->GetNameData();
	EditorUI::TextField("", nameData, 100);
	if (EditorUI::ItemAction(EditorUI::ActionType::Selected))
	{
		if (!m_isSceneNameFieldSelected)
		{
			m_oldPath = (*m_pScene)->GetPath();
			m_isSceneNameFieldSelected = true;
		}
	}
	(*m_pScene)->SetName(nameData);

	if (!EditorUI::ItemAction(EditorUI::ActionType::Selected))
	{

		if (m_isSceneNameFieldSelected)
		{
			m_pSceneManager->ChangeSceneName((*m_pScene));
			const char* test = (*m_pScene)->GetNameData();
			m_pFilebrowser->RenameFile(m_oldPath.string().c_str(), test);
			m_isSceneNameFieldSelected = false;
		}

	}
}

void EditorLich::HierachyWindow::DeselectingObject()
{
	EditorUI::Button("button", EditorUI::GetWindowSizeAvailable(), true);
	const bool is_hovered = EditorUI::ItemAction(EditorUI::ActionType::Hovered);
	if (ImGui::IsMouseReleased(EditorUI::MouseButtonUI::LeftButton) && is_hovered)
	{
		m_hasItemSelected = true;
		m_pData = nullptr;
		m_type = ObjectType::NONE;
	}
}

void EditorLich::HierachyWindow::RegisterHierachyInput(Physics::Transform* transform)
{
	std::vector<Physics::Transform*>* ptrArray = (m_pSceneGraph->GetTransformArray());
	if (EditorUI::ItemAction(EditorUI::ActionType::Hovered))
	{
		if (EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::LeftButton) || EditorUI::ItemAction(EditorUI::ActionType::Clicked, EditorUI::MouseButtonUI::RightButton))
		{
			m_hasItemSelected = true;
			m_pData = (*ptrArray)[transform->GetSceneIndex()]->GetGameObject();
			m_gameObjectSelected = (*ptrArray)[transform->GetSceneIndex()]->GetGameObject();
			m_type = ObjectType::GAMEOBJECT;
			m_pSceneGraph->GetScene()->GetGizmos()->SetGO((*ptrArray)[transform->GetSceneIndex()]->GetGameObject());
		}
	}

	if (EditorUI::ItemAction(EditorUI::ActionType::Active) && !EditorUI::ItemAction(EditorUI::ActionType::Hovered))
	{
		if (m_prevMousePos == Vector2D::zero) m_prevMousePos = EditorUI::GetMousePos();

		if (!m_hasDragDrop)
		{
			m_startPos = EditorUI::GetMousePos();
			m_startMousePos = EditorUI::GetMousePos();
			m_hasDragDrop = true;
			m_currentIndex = transform->GetSceneIndex();
		}
		Vector2D pos = EditorUI::GetMousePos();


		RegisterSwapInput(pos);
		RegisterChildParentInput(pos);

		m_prevMousePos = pos;
	}
}

void EditorLich::HierachyWindow::RegisterSwapInput(Vector2D pos)
{
	float deltaPos = Absolute<float>(pos.y - m_startPos.y);
	int sign = Sign<int>((pos.y - m_startPos.y));

	m_endMousePos = pos.y;
	if (deltaPos < 16) return;

	m_hierachyMouvement += sign;
	m_startPos.y = EditorUI::GetMousePos().y;
}

void EditorLich::HierachyWindow::RegisterChildParentInput(Vector2D pos)
{
	if (pos.x - m_startPos.x > m_pixelSizeToAddChild)
	{
		m_childAction = ADD;
	}
	if (pos.x - m_startPos.x < -m_pixelSizeToAddChild)
	{
		m_childAction = REMOVE;
	}
}
