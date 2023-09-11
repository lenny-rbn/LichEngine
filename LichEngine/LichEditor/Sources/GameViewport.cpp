#include <Core/App.h>
#include <Editor/GameViewport.h>
#include <Maths.h>


namespace EditorLich
{
	GameViewport::GameViewport(Renderer::RenderManager* renderManager, Core::App* app, SceneManagement::SceneManager* scenManagerPtr)
	: m_pApp(app), m_pRenderManager(renderManager), m_pSceneManager(scenManagerPtr)
	{
		m_size = Vector2D(1920, 1080);
		m_prevSize = m_size;
		m_windowSize = Vector2D::zero;
		m_pCamEditor = new SceneManagement::GameObject();
		m_pCam = m_pCamEditor->AddComponent<CameraRender::Camera>();
		m_pCam->SetSkybox(m_pSceneManager->GetSceneSkybox());
		m_pSceneManager->SetNewCamera(m_pCam);
	}

	GameViewport::~GameViewport()
	{
		delete m_pCamEditor;
	}

	void GameViewport::Update()
	{
		m_pCam->SetSkybox(m_pSceneManager->GetSceneSkybox());
		EditorUI::WindowFlags window_flags = EditorUI::WindowOptions::NoScrollBar | EditorUI::WindowOptions::NoScrollWithMouse;
		EditorUI::PushStyle(EditorUI::ColorObject::ChildBg, Vector4D(0.2f, 0.2f, 0.2f, 1));

		if (EditorUI::CreateWindowUI(m_windowName, &m_isOpen, window_flags))
		{
			if (ResizeViewport())
			{
				EditorUI::PopStyle(EditorUI::StyleType::Color);
				return;
			}

			if (!m_pSceneManager->GetCurrentScene()->IsPlaying())
			{
			CameraInputManagement();
			}
			else if ((*m_pApp->GetInputManager())->IsPressed("Escape"))
			{
				m_pSceneManager->ReloadCurrentScene();
				WindowAPI::SetInputMode((*m_pApp->GetInputManager())->GetWindow(), Input::CURSOR, Mode::NORMAL);
			}

			m_pCamEditor->GetTransform()->ResetUpdateState();
			m_pCam->CalculateMatrix();
			m_pCamEditor->GetTransform()->Update(m_pCam, false, true);

			m_prevMousePos = EditorUI::GetMousePos();

			SceneManagement::Gizmos* gizmos = (*m_pApp->GetSceneManager())->GetCurrentScene()->GetGizmos();

			// Game viewport  Mouvement Button 
			if (EditorUI::Button("Position", Vector2D(40, 40)))
				gizmos->SetModificationMode(SceneManagement::GizmosModificationMode::POSITION);

			EditorUI::SameLine();
			if (EditorUI::Button("Rotation", Vector2D(40, 40)))
				gizmos->SetModificationMode(SceneManagement::GizmosModificationMode::ROTATION);

			EditorUI::SameLine();
			if (EditorUI::Button("Scale", Vector2D(40, 40)))
				gizmos->SetModificationMode(SceneManagement::GizmosModificationMode::SCALE);

			EditorUI::SameLine();
			EditorUI::CenterObject(20.f);
			Resources::Texture* tex = (Resources::Texture*)Resources::ResourceManager::GetSpecificResource(Resources::TEXTURE, "Play");
			if(tex)
				tex->IsCreated();
			
			if (EditorUI::ImageButton((EditorUI::TextureID)tex->GetTexture(), Vector2D(20, 20)))
			{
				m_pSceneManager->ChangeCurrentScenePlayingState();

				if (!m_pSceneManager->IsCurrentSceneIsPlaying())
					m_pSceneManager->SetNewCamera(m_pCam);
				else
					m_pSceneManager->UseSceneCamera();
			}
			else if (!m_pSceneManager->IsCurrentSceneIsPlaying())
			{
				m_pSceneManager->SetNewCamera(m_pCam);
			}

			EditorUI::SameLine();
			if (EditorUI::Button("Draw All Colliders", Vector2D(40, 40)))
			{
				(*m_pSceneManager->GetCurrentScenePtr())->ChangeDrawCollidersState();
			}

			EditorUI::ImageField((EditorUI::TextureID)m_pRenderManager->GetRenderTexture(), m_size, Vector2D(0, 1), Vector2D(1, 0));
			EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
		}
		else
		{
			EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
		}
		EditorUI::PopStyle(EditorUI::StyleType::Color);
	}

	bool GameViewport::ResizeViewport()
	{
		if (EditorUI::GetWindowSize() != m_windowSize)
		{
			DrawSceneViewport();
			return true;
		}

		if (!m_isResize) return false;

		m_isResize = false;
		CalculateResize();

		return false;
	}

	void GameViewport::DrawSceneViewport()
	{
		m_windowSize = EditorUI::GetWindowSize();
		m_isResize = true;

		EditorUI::ImageField((EditorUI::TextureID)m_pRenderManager->GetRenderTexture(), m_size, Vector2D(0, 1), Vector2D(1, 0));
		EditorUI::End(EditorUI::TypeUI::Window, __FILE__, __LINE__);
	}

	void  GameViewport::CalculateResize()
	{
		if (m_size.y == m_prevSize.y)
		{
			m_size.x = m_windowSize.x;
			m_size.y = m_windowSize.x / 1.77777777778f;
			if (m_size.y > m_windowSize.y)
			{
				m_size.y = m_windowSize.y;
				m_size.x = m_size.y * 1.77777777778f;
			}
			m_prevSize = m_size;
			return;
		}

		if (m_size.x == m_prevSize.x)
		{
			m_size.y = m_windowSize.y ;
			m_size.x = m_size.y * 1.77777777778f;
			m_prevSize = m_size;
		}
	}

	void GameViewport::CameraInputManagement()
	{
		Core::InputManager* inputManager = (*m_pApp->GetInputManager());
		Core::TimeManager* timeManager = (*m_pApp->GetTimeManager());
		Physics::Transform* transform = m_pCamEditor->GetTransform();
		Vector2D currentMousePos = EditorUI::GetMousePos();

		if (IsInWindow(currentMousePos))
		{
			if (inputManager->IsContinue("Front"))
			{
				transform->Translate(transform->GetVectorForward() * timeManager->GetDeltaTime() * 8.f);
			}

			if (inputManager->IsContinue("Back"))
			{
				transform->Translate(transform->GetVectorForward() * timeManager->GetDeltaTime() * -8.f);
			}

			if (inputManager->IsContinue("Right"))
			{
				transform->Translate(transform->GetVectorRight() * timeManager->GetDeltaTime() * -8.f);
			}

			if (inputManager->IsContinue("Left"))
			{
				transform->Translate(transform->GetVectorRight() * timeManager->GetDeltaTime() * 8.f);
			}

			if (inputManager->IsContinue("Down"))
			{
				transform->Translate(Vector3D::up * timeManager->GetDeltaTime() * 8.f);
			}

			if (inputManager->IsContinue("Up"))
			{
				transform->Translate(Vector3D::up * timeManager->GetDeltaTime() * -8.f);
			}
		
			if (inputManager->IsContinue("Mouse0"))
			{
				HandleGizmosInputs(currentMousePos);
			}
			else if (inputManager->IsPressed("Mouse0"))
			{
				Vector4D id = m_pSceneManager->GetIDFromPicking(currentMousePos, EditorUI::GetWindowPos(), m_size);
				SceneManagement::GameObject* gO = m_pSceneManager->GetGameObjectFromID(id);

				axe = AxeSelected::NONE;
				if (id != Vector4D(255.f, 255.f, 253.f, 255.f) && id != Vector4D(255.f, 255.f, 254.f, 255.f) && id != Vector4D(255.f, 255.f, 255.f, 255.f) && gO != nullptr)
				{
					m_pHierachyWindow->SetItemSelected(gO);
					return;
				}
				if (id == Vector4D(255.f, 255.f, 253.f, 255.f))
				{
					axe = AxeSelected::Y;
					return;
				}
				if (id == Vector4D(255.f, 255.f, 254.f, 255.f))
				{
					axe = AxeSelected::Z;
					return;
				}
				if (id == Vector4D(255.f, 255.f, 255.f, 255.f))
				{
					axe = AxeSelected::X;
					return;
				}
			}
			else if (inputManager->IsReleased("Mouse0"))
			{
				axe = AxeSelected::NONE;
			}
			
			if (inputManager->IsContinue("Mouse1"))
			{
				float dotForward = DotProduct3D(Vector3D::forward, transform->GetVectorForward());
				float dotRight = DotProduct3D(Vector3D::forward, transform->GetVectorRight());

				Vector3D eulerAngle = transform->GetGlobalRotation();

				float rotationX = (currentMousePos.y - m_prevMousePos.y) * timeManager->GetDeltaTime() * 30.f;
				float rotationY = (currentMousePos.x - m_prevMousePos.x) * timeManager->GetDeltaTime() * -30.f;

				eulerAngle.x += rotationX;
				eulerAngle.y += rotationY;

				if (eulerAngle.x < -89.f)
					eulerAngle.x = -89.f;
				else if (eulerAngle.x > 89.f)
					eulerAngle.x = 89.f;

				transform->SetGlobalRotation(eulerAngle);
			}

			if (inputManager->IsPressed("1"))
			{
				transform->SetGlobalPosition(Vector3D(0));
				transform->SetGlobalRotation(Vector3D(0));
			}

			if (inputManager->IsPressed("2"))
			{
				m_pSceneManager->GetCurrentScene()->GetGizmos()->SetModificationMode(SceneManagement::GizmosModificationMode::POSITION);
			}

			if (inputManager->IsPressed("3"))
			{
				m_pSceneManager->GetCurrentScene()->GetGizmos()->SetModificationMode(SceneManagement::GizmosModificationMode::ROTATION);
			}

			if (inputManager->IsPressed("4"))
			{
				m_pSceneManager->GetCurrentScene()->GetGizmos()->SetModificationMode(SceneManagement::GizmosModificationMode::SCALE);
			}
		}
		m_prevMousePos = currentMousePos;
	}

	void GameViewport::HandleGizmosInputs(Vector2D mousePosition)
	{
		Core::TimeManager* timeManager = (*m_pApp->GetTimeManager());
		float deltaTime = timeManager->GetDeltaTime();

		if (axe == AxeSelected::NONE)
			return;

		SceneManagement::Gizmos* gizmos = m_pSceneManager->GetCurrentScene()->GetGizmos();
		CameraRender::Camera* cam = m_pCam;
		Physics::Transform* camTransform = m_pCamEditor->GetTransform();
		Physics::Transform* transform = gizmos->GetGO()->GetTransform();

		Vector2D windowPos = EditorUI::GetWindowPos();
		Vector2D windowSize = EditorUI::GetWindowSize();

		Vector2D newPos = ((mousePosition - windowPos) / windowSize.x * 1920.f, (mousePosition - windowPos) / windowSize.y * 1080.f);
		Vector2D oldPos = ((m_prevMousePos - windowPos) / windowSize.x * 1920.f, (m_prevMousePos - windowPos) / windowSize.y * 1080.f);

		Vector2D posMouseClamp = ((mousePosition.x - windowPos.x) / windowSize.x, (mousePosition.x - windowPos.x) / windowSize.y);
		Vector2D oldPosMouseClamp = ((m_prevMousePos.y - windowPos.y) / windowSize.x, (m_prevMousePos.y - windowPos.y) / windowSize.y);

		Vector3D newRay = Vector3D::Normalize(cam->GetRayFromScreenCoord(newPos));
		Vector3D oldRay = Vector3D::Normalize(cam->GetRayFromScreenCoord(oldPos));
		Vector3D worldNewPos, worldOldPos;

		switch (gizmos->GetModificationMode())
		{
		case SceneManagement::GizmosModificationMode::POSITION:
		{
			Vector3D p0 = transform->GetGlobalPosition();
			Vector3D p1 = transform->GetGlobalPosition();

			if (axe == AxeSelected::Y)
				p1 += (transform->GetVectorUp() * 10.f);
			else if (axe == AxeSelected::Z)
				p1 += (transform->GetVectorForward() * 10.f);
			else if (axe == AxeSelected::X)
				p1 += (transform->GetVectorRight() * 10.f);

			Vector2D direction = m_pCam->WorldToScreenCoords(p1) - m_pCam->WorldToScreenCoords(p0);
			direction.Normalize();

			Vector2D mouseDirection = Vector2D(mousePosition.x - m_prevMousePos.x, -(mousePosition.y - m_prevMousePos.y));
			float magnitude = mouseDirection.Magnitude();
			if (magnitude != 0.f)
				mouseDirection.Normalize();

			float dot = DotProduct2D(direction, mouseDirection);
			float scale = (transform->GetGlobalPosition() - camTransform->GetGlobalPosition()).Magnitude() / 10.f;

			if (abs(dot) < 0.85f) break;

			if (axe == AxeSelected::Y)
				transform->Translate(transform->GetVectorUp() * dot * magnitude * scale * deltaTime * 3.f);
			else if (axe == AxeSelected::Z)
				transform->Translate(transform->GetVectorForward() * dot * magnitude * scale * deltaTime * 3.f);
			else if (axe == AxeSelected::X)
				transform->Translate(transform->GetVectorRight() * dot * magnitude * scale * deltaTime * 3.f);
			break;
		}
		case SceneManagement::GizmosModificationMode::ROTATION:
			if (axe == AxeSelected::Y)
			{
				worldNewPos = GetPointInPlaneFromRay(newRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorUp());
				worldOldPos = GetPointInPlaneFromRay(oldRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorUp());

				Vector3D vec1 = Vector3D::Normalize(worldNewPos - transform->GetGlobalPosition());
				Vector3D vec2 = Vector3D::Normalize(worldOldPos - transform->GetGlobalPosition());

				float angle = atan2f(DotProduct3D(CrossProduct(vec2, vec1), transform->GetVectorUp()), DotProduct3D(vec1, vec2)) * deltaTime * 300.f;
				Quaternion quat = Quaternion::AngleAxisToQuat(transform->GetVectorUp(), angle * 20.f);
				transform->SetGlobalRotation(quat * transform->GetGlobalRotationInQuaternion());
			}
			else if (axe == AxeSelected::Z)
			{
				worldNewPos = GetPointInPlaneFromRay(newRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorForward());
				worldOldPos = GetPointInPlaneFromRay(oldRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorForward());

				Vector3D vec1 = Vector3D::Normalize(worldNewPos - transform->GetGlobalPosition());
				Vector3D vec2 = Vector3D::Normalize(worldOldPos - transform->GetGlobalPosition());

				float angle = atan2f(DotProduct3D(CrossProduct(vec2, vec1), transform->GetVectorForward()), DotProduct3D(vec1, vec2)) * deltaTime * 300.f;
				Quaternion quat = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), angle * 20.f);
				transform->SetGlobalRotation(quat * transform->GetGlobalRotationInQuaternion());
			}
			else if (axe == AxeSelected::X)
			{
				worldNewPos = GetPointInPlaneFromRay(newRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorRight());
				worldOldPos = GetPointInPlaneFromRay(oldRay, camTransform->GetGlobalPosition(), transform->GetGlobalPosition(), transform->GetVectorRight());

				Vector3D vec1 = Vector3D::Normalize(worldNewPos - transform->GetGlobalPosition());
				Vector3D vec2 = Vector3D::Normalize(worldOldPos - transform->GetGlobalPosition());

				float angle = atan2f(DotProduct3D(CrossProduct(vec2, vec1), transform->GetVectorRight()), DotProduct3D(vec1, vec2)) * deltaTime * 300.f;
				Quaternion quat = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), angle * 20.f);
				transform->SetGlobalRotation(quat * transform->GetGlobalRotationInQuaternion());
			}
			break;
		case SceneManagement::GizmosModificationMode::SCALE:
		{
			Vector2D mouseDirection = Vector2D(mousePosition.x - m_prevMousePos.x, -(mousePosition.y - m_prevMousePos.y));
			Vector3D p0 = transform->GetGlobalPosition();
			Vector3D p1 = transform->GetGlobalPosition();;

			if (axe == AxeSelected::Y)
				p1 += (transform->GetVectorUp() * 10.f);
			else if (axe == AxeSelected::Z)
				p1 += (transform->GetVectorForward() * 10.f);
			else if (axe == AxeSelected::X)
				p1 += (transform->GetVectorRight() * 10.f);

			Vector2D direction = m_pCam->WorldToScreenCoords(p1) - m_pCam->WorldToScreenCoords(p0);
			direction.Normalize();
			float dist = (transform->GetGlobalPosition() - camTransform->GetGlobalPosition()).Magnitude();
			std::cout << direction.x << ",  " << direction.y << "\n";
			std::cout << dist << "\n";


			float magnitude = mouseDirection.Magnitude();
			if (magnitude != 0.f)
				mouseDirection.Normalize();

			float dot = DotProduct2D(direction, mouseDirection);
			float scale = (transform->GetGlobalPosition() - camTransform->GetGlobalPosition()).Magnitude() * 0.1f;
			if (scale < 1.0f)
				scale = 1.0f;

			if (abs(dot) <= 0.85f) break;

			if (axe == AxeSelected::Y)
				transform->Scale(Vector3D(1.f, 1.f + dot * magnitude * deltaTime * 2.f, 1.f));
			else if (axe == AxeSelected::Z)
				transform->Scale(Vector3D(1.f, 1.f, 1.f + dot * magnitude * deltaTime * 2.f));
			else if (axe == AxeSelected::X)
				transform->Scale(Vector3D(1.f + dot * magnitude * deltaTime * 2.f, 1.f, 1.f));

			break;
		}
		default:
			break;
		}
	}
}