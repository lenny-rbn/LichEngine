#include <pch.h>
#include <Renderer/Mesh.h>
#include <Core/WrapperWindow.h>
#include <Resources/ResourceManager.h>
#include <Resources/Material.h>
#include <Animation/Animator.h>
#include <Scene/GameObject.h>
#include <Camera/Skybox.h>
#include <Physics/Collider.h>
#include <Scene/SceneGraph.h>
#include <UI/Text.h>


namespace WrapperRHI
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
	}

	int RHI::InitOpenGL()
	{
		gladLoadGL();
		if (!gladLoadGLLoader((GLADloadproc)WindowAPI::GetProcdAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return 1;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		return 0;
	}

	void RHI::SetViewportSize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void RHI::SetViewportSize(Vector2D size)
	{
		glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
	}

	void RHI::EnableOptions(GProperties toEnable)
	{
		glEnable(toEnable);
	}

	void RHI::DisableOptions(GProperties toDisable)
	{
		glDisable(toDisable);
	}

	void RHI::DrawMeshShadowPass(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource)
	{
		if (!info->mesh->isEnable || !info->mesh->HasMaterialsGeneratingShadow()) return;

		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);
		SendMat4ToShader(shader, "model", 1, false, &info->model.e[0]);

		if (info->animator)
		{
			Send1iToShader(shader, "isAnimated", info->animator->IsReady());
			std::vector<Matrix4D> matrices = info->animator->GetMatrices();
			std::string mat = "boneMatrices[";

			for (int j = 0; j < matrices.size(); j++)
				SendMat4ToShader(shader, (mat + std::to_string(j) + ']').c_str(), 1, false, &matrices[j].e[0]);
		}
		else
		{
			Send1iToShader(shader, "isAnimated", false);
		}

		for (int i = 0; i < info->mesh->GetModel()->GetSubModels().size(); i++)
		{
			Resources::Material* mat = info->mesh->GetMaterial(i);
			Resources::SubModel* pSubModel = info->mesh->GetModel()->GetSubModels()[i];
			if(mat != nullptr && mat->IsGeneratingShadow())
				Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
		}
	}

	void RHI::DrawMeshNotAnimated(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource)
	{
	
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);
		SendMat4ToShader(shader, "model", 1, false, &info->model.e[0]);
		SendMat4ToShader(shader, "mvp", 1, false, &info->mvp.e[0]);
		Send1iToShader(shader, "isAnimated", false);

		for (int i = 0; i < info->mesh->GetModel()->GetSubModels().size(); i++)
		{
			Resources::SubModel* pSubModel = info->mesh->GetModel()->GetSubModels()[i];
			Resources::Material* mat = info->mesh->GetMaterial(i);
			SendMaterialToShader(shader, mat);
			Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
		}
	}


	void RHI::DrawMesh(SceneManagement::ObjInfo* info, Resources::Resource* shaderResource)
	{

		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);
		SendMat4ToShader(shader, "model", 1, false, &info->model.e[0]);
		SendMat4ToShader(shader, "mvp", 1, false, &info->mvp.e[0]);

		if (info->animator)
		{
			
			Send1iToShader(shader, "isAnimated", true);
			std::vector<Matrix4D> matrices = info->animator->GetMatrices();
			std::string mat = "boneMatrices[";

			for (int j = 0; j < matrices.size(); j++)
				SendMat4ToShader(shader, (mat + std::to_string(j) + ']').c_str(), 1, false, &matrices[j].e[0]);
		}
		else
		{
			Send1iToShader(shader, "isAnimated", false);
		}

		for (int i = 0; i < info->mesh->GetModel()->GetSubModels().size(); i++)
		{
			Resources::SubModel* pSubModel = info->mesh->GetModel()->GetSubModels()[i];
			Resources::Material* mat = info->mesh->GetMaterial(i);
			SendMaterialToShader(shader, mat);
			Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
		}
	}

	void RHI::SendMaterialToShader(Resources::Resource* shaderResource, Resources::Resource* materialResource)
	{
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);
		Resources::Material* material = static_cast<Resources::Material*>(materialResource);

		if (material == nullptr || !material->IsCreated())
			material = Resources::ResourceManager::GetResource<Resources::Material>("defaultMat");

		Resources::Texture* diffuse = material->GetDiffuseMap();
		Resources::Texture* normal = material->GetNormalMap();
		Resources::Texture* height = material->GetHeightMap();
		Resources::Texture* metallic = material->GetMetallicMap();
		Resources::Texture* roughness = material->GetRoughnessMap();
		Resources::Texture* ao = material->GetAOMap();

		if (diffuse != nullptr && diffuse->IsCreated())
		{
			ActivateAndBindTexture2D(0, diffuse->GetTexture());
			Send1iToShader(shader, "handleDiffuse", true);
		}
		else
		{
			Send4fToShader(shader, "albedo", material->GetAlbedo());
			Send1iToShader(shader, "handleDiffuse", false);
		}

		if (normal != nullptr && normal->IsCreated())
		{
			ActivateAndBindTexture2D(1, normal->GetTexture());
			Send1iToShader(shader, "handleNormal", true);
		}
		else
		{
			Send1iToShader(shader, "handleNormal", false);
		}

		if (height != nullptr && height->IsCreated())
		{
			ActivateAndBindTexture2D(2, height->GetTexture());
			Send1iToShader(shader, "handleParallax", true);
		}
		else
		{
			Send1iToShader(shader, "handleParallax", false);
		}

		if (metallic != nullptr && metallic->IsCreated())
		{
			ActivateAndBindTexture2D(3, metallic->GetTexture());
			Send1iToShader(shader, "handleMetallic", true);
		}
		else
		{
			Send1fToShader(shader, "metallic", material->GetMetalness());
			Send1iToShader(shader, "handleMetallic", false);
		}

		if (roughness != nullptr && roughness->IsCreated())
		{
			ActivateAndBindTexture2D(4, roughness->GetTexture());
			Send1iToShader(shader, "handleRoughness", true);
		}
		else
		{
			Send1fToShader(shader, "roughness", material->GetRoughness());
			Send1iToShader(shader, "handleRoughness", false);
		}

		if (ao != nullptr && ao->IsCreated())
		{
			ActivateAndBindTexture2D(5, ao->GetTexture());
			Send1iToShader(shader, "handleAO", true);
		}
		else
		{
			Send1iToShader(shader, "handleAO", false);
		}

		Send1iToShader(shader, "bloom", material->IsBloomActive());
		if(material->IsUsingDiffuseTextForOpacity())
			Send1fToShader(shader, "opacity", 1.0f);
		else
			Send1fToShader(shader, "opacity", material->GetOpacity());
	}

	void RHI::DrawSkyBox(Resources::Resource* ressourceSkybox)
	{
		CameraRender::SkyBox* skybox = static_cast<CameraRender::SkyBox*>(ressourceSkybox);

		SetCullFace(FRONT);
		SetDepthFunc(LEQUAL);
		ActivateAndBindCubemap(0, skybox->GetCubeMap());
		Draw(skybox->GetModel()->GetSubModels()[0]->VAO, (sizei)skybox->GetModel()->GetSubModels()[0]->indices.size());
		SetDepthFunc(LESS);
		SetCullFace(BACK);
	}

	void RHI::DrawTextUI(Resources::Resource* shaderResource, std::string text, Resources::Resource* fontResource, Vector2D pos, float scale, Vector4D color, uint& UIVAO, uint& UIVBO)
	{
		UI::Font* font = static_cast<UI::Font*>(fontResource);
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);

		EnableOptions(BLEND);
		SetBlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
		DisableOptions(DEPTH_TEST);

		Send4fToShader(shader, "textColor", color);
		glBindVertexArray(UIVAO);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			UI::Character character = font->GetCharacter(*c);

			float xpos = pos.x + character.bearing.x * scale;
			float ypos = pos.y - (character.size.y - character.bearing.y) * scale;

			float w = character.size.x * scale;
			float h = character.size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			ActivateAndBindTexture2D(0, character.textureID);
			glBindBuffer(GL_ARRAY_BUFFER, UIVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos.x += (character.advance >> 6) * scale;
		}
		glBindVertexArray(0);

		DisableOptions(BLEND);
		EnableOptions(DEPTH_TEST);
	}

	void RHI::DrawMeshForPicking(Renderer::Mesh* mesh, Physics::Transform* transform, Resources::Resource* shaderResource)
	{
		if (transform->GetLocalScale().x == 0)
			return;

		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);

		if (transform->GetGameObject()->HasComponent<AnimationSystem::Animator>() && transform->GetGameObject()->GetComponent<AnimationSystem::Animator>()->IsReady())
		{
			AnimationSystem::Animator* animator = transform->GetGameObject()->GetComponent<AnimationSystem::Animator>();
			Send1iToShader(shader, "isAnimated", animator->IsReady());
			std::vector<Matrix4D> matrices = animator->GetMatrices();
			std::string mat = "boneMatrices[";

			for (int j = 0; j < matrices.size(); j++)
				SendMat4ToShader(shader, (mat + std::to_string(j) + ']').c_str(), 1, false, &matrices[j].e[0]);
		}
		else
		{
			Send1iToShader(shader, "isAnimated", false);
		}

		Send4iToShader(shader, "id", transform->GetGameObject()->GetId());

		for (int i = 0; i < mesh->GetModel()->GetSubModels().size(); i++)
		{
			Resources::SubModel* pSubModel = mesh->GetModel()->GetSubModels()[i];
			SendMat4ToShader(shader, "mvp", 1, false, &transform->GetMVP().e[0]);
			Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
		}
	}

	void RHI::DrawGizmos(Physics::Transform* transform, int modificationMode, Physics::Transform* cameraTransform, Resources::Resource* shaderResource)
	{
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);

		Resources::Model* model;
		Matrix4D modelMatrices[3];
		Matrix4D mvp[3];
		Vector3D color[3] = { Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1) };
		Vector3D position = transform->GetGlobalPosition();
		Vector3D rotation = transform->GetGlobalRotation();

		if (modificationMode == 1)
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoCircle");
			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(3.f, 6.f, 3.f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Quaternion quatY = transform->GetGlobalRotationInQuaternion();
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), 90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), -90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}
		else if(modificationMode == 0)
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoArrow");

			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(6.f, 2.f, 6.f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Quaternion quatY = transform->GetGlobalRotationInQuaternion();
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), -90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), 90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}
		else
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoScale");

			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(4.5f, 4.5f, 4.5f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Vector3D rot = transform->GetGlobalRotation();
			Quaternion quatY = Quaternion::GetQuaternionFromEulerAngles(ClampRotation(rot));
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), -90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), 90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}

		for (int i = 0; i < 3; i++)
			mvp[i] = cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetVPMatrix() * modelMatrices[i];

		DisableOptions(DEPTH_TEST);
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < model->GetSubModels().size(); i++)
			{
				Resources::SubModel* pSubModel = model->GetSubModels()[i];

				SendMat4ToShader(shader, "model", 1, false, &modelMatrices[j].e[0]);
				SendMat4ToShader(shader, "mvp", 1, false, &mvp[j].e[0]);
				Send3fToShader(shader, "color", color[j]);

				Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
			}
		}
		EnableOptions(DEPTH_TEST);
	}

	void RHI::DrawImageUI(uint texture, uint UIImageVAO, uint UIImageVBO)
	{
		DisableOptions(DEPTH_TEST);
		glBindVertexArray(UIImageVAO);
		ActivateAndBindTexture2D(0, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		EnableOptions(DEPTH_TEST);
	}

	void RHI::DrawGizmosForPicking(Physics::Transform* transform, int modificationMode, Physics::Transform* cameraTransform, Resources::Resource* shaderResource)
	{
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);
		Send1iToShader(shader, "isAnimated", false);

		Resources::Model* model;
		Matrix4D modelMatrices[3];
		Matrix4D mvp[3];
		Vector3D color[3] = { Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1) };
		Vector3D position = transform->GetGlobalPosition();
		Vector3D rotation = transform->GetGlobalRotation();

		if (modificationMode == 1)
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoCircle");
			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(3.f, 6.f, 3.f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Quaternion quatY = transform->GetGlobalRotationInQuaternion();
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), 90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), -90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}
		else if (modificationMode == 0)
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoArrow");

			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(6.f, 2.f, 6.f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Quaternion quatY = transform->GetGlobalRotationInQuaternion();
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), -90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), 90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}
		else
		{
			model = Resources::ResourceManager::GetResource<Resources::Model>("GizmoScale");

			if (!model->IsCreated())
				return;

			Vector3D scale = Vector3D(4.5f, 4.5f, 4.5f) * ((position - cameraTransform->GetGlobalPosition()).Magnitude() / 10.f);

			Vector3D rot = transform->GetGlobalRotation();
			Quaternion quatY = Quaternion::GetQuaternionFromEulerAngles(ClampRotation(rot));
			Quaternion quatX = Quaternion::AngleAxisToQuat(transform->GetVectorForward(), -90.f) * quatY;
			Quaternion quatZ = Quaternion::AngleAxisToQuat(transform->GetVectorRight(), 90.f) * quatY;

			modelMatrices[0] = Matrix4D::CreateTransformMatrix(position, quatY, scale);
			modelMatrices[1] = Matrix4D::CreateTransformMatrix(position, quatZ, scale);
			modelMatrices[2] = Matrix4D::CreateTransformMatrix(position, quatX, scale);
		}

		for (int i = 0; i < 3; i++)
			mvp[i] = cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetVPMatrix() * modelMatrices[i];

		Vector4D ids[3] = { Vector4D(255.f, 255.f, 253.f, 255.f), Vector4D(255.f, 255.f, 254.f, 255.f), Vector4D(255.f,255.f,255.f,255.f) };

		WrapperRHI::RHI::DisableOptions(DEPTH_TEST);
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < model->GetSubModels().size(); i++)
			{
				Resources::SubModel* pSubModel = model->GetSubModels()[i];

				Send4iToShader(shader, "id", ids[j]);
				SendMat4ToShader(shader, "mvp", 1, false, &mvp[j].e[0]);
				Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
			}
		}
		WrapperRHI::RHI::EnableOptions(DEPTH_TEST);
	}

	void RHI::DrawCollider(SceneManagement::GameObject* gameObject, Physics::Transform* cameraTransform, Resources::Resource* shaderResource)
	{
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);

		Physics::Transform* transform = gameObject->GetTransform();
		Physics::Collider* collider = gameObject->GetComponent<Physics::Collider>();

		Resources::Model* model = nullptr;
		Vector3D scale;

		Physics::ShapeType shape = collider->GetShapeType();
		if (shape == Physics::ShapeType::Box)
		{
			scale = transform->GetGlobalScale() * 2.f;
			scale.x *= collider->GetScale().x;
			scale.y *= collider->GetScale().y;
			scale.z *= collider->GetScale().z;
			model = Resources::ResourceManager::GetResource<Resources::Model>("Cube");
		}
		else if (shape == Physics::ShapeType::Sphere)
		{
			scale = transform->GetGlobalScale() * collider->GetScale().x * 0.5f;
			model = Resources::ResourceManager::GetResource<Resources::Model>("Sphere");
		}
		else if (shape == Physics::ShapeType::Capsule)
		{
			scale = transform->GetGlobalScale();
			scale.x *= collider->GetScale().x;
			scale.y *= collider->GetScale().y;
			scale.z *= collider->GetScale().x;
			model = Resources::ResourceManager::GetResource<Resources::Model>("Capsule");
		}

		if (model == nullptr || !model->IsCreated())
			return;

		Matrix4D modelMatrix;
		Matrix4D mvp;
		Vector3D color = Vector3D(0, 1, 0);
		Vector3D position = transform->GetGlobalPosition() + collider->GetOffset();
		Vector3D rotation = transform->GetGlobalRotation();
		Quaternion quat = Quaternion::GetQuaternionFromEulerAngles(Vector3D((rotation.x), (rotation.y), (rotation.z)));

		modelMatrix = Matrix4D::CreateTransformMatrix(position, quat, scale);
		mvp = cameraTransform->GetGameObject()->GetComponent<CameraRender::Camera>()->GetVPMatrix() * modelMatrix;

		DrawMode(GProperties::FRONT_AND_BACK, GProperties::DRAWLINE);
		WrapperRHI::RHI::DisableOptions(DEPTH_TEST);
		for (int i = 0; i < model->GetSubModels().size(); i++)
		{
			Resources::SubModel* pSubModel = model->GetSubModels()[i];

			SendMat4ToShader(shader, "model", 1, false, &modelMatrix.e[0]);
			SendMat4ToShader(shader, "mvp", 1, false, &mvp.e[0]);
			Send3fToShader(shader, "color", color);

			Draw(pSubModel->VAO, (sizei)pSubModel->indices.size());
		}
		WrapperRHI::RHI::EnableOptions(DEPTH_TEST);
		DrawMode(GProperties::FRONT_AND_BACK, GProperties::DRAWFILL);
	}

	void RHI::Create(const ResourceGraphicType type, Resources::Resource* resource)
	{
		switch (type)
		{
		case ResourceGraphicType::TEXTURE:
			{
				Resources::Texture* texture = static_cast<Resources::Texture*>(resource);
				CreateTexture(texture->GetData(), texture->GetNrChannels(), texture->GetWidth(), texture->GetHeight(), texture->GetTexture(), texture->GetSampler(), texture->DoGammaCorrection());
				break;
			}
			case ResourceGraphicType::SHADER:
			{
				Resources::Shader* shader = static_cast<Resources::Shader*>(resource);
				CreateShader(shader->GetShaderProgram(), shader->GetDatas(), shader->GetShaderTypes());
				break;
			}
			case ResourceGraphicType::MODEL:
			{
				Resources::Model* model = static_cast<Resources::Model*>(resource);

				for (int i = 0; i < model->GetSubModels().size(); i++)
				{
					Resources::SubModel* pSubModel = model->GetSubModels()[i];
					if(pSubModel->vertices.size() != 0)
						CreateModel(pSubModel->VBO, pSubModel->VAO, pSubModel->EBO, pSubModel->vertices, pSubModel->indices);
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}

	void RHI::Delete(const ResourceGraphicType type, Resources::Resource* resource)
	{
		switch (type)
		{
			case ResourceGraphicType::TEXTURE:
			{
				Resources::Texture* texture = static_cast<Resources::Texture*>(resource);
				DeleteTexture(texture->GetTexture(), texture->GetSampler());
				break;
			}
			case ResourceGraphicType::SHADER:
			{
				Resources::Shader* shader = static_cast<Resources::Shader*>(resource);
				DeleteShader(shader->GetShaderProgram());
				break;
			}
			case ResourceGraphicType::MODEL:
			{
				Resources::Model* model = static_cast<Resources::Model*>(resource);
				for (int i = 0; i < model->GetSubModels().size(); i++)
				{
					Resources::SubModel* pSubModel = model->GetSubModels()[i];
					DeleteModel(pSubModel->VBO, pSubModel->VAO);
				}
				break;
			}
			case ResourceGraphicType::SKYBOX :
			{
				CameraRender::SkyBox* skybox = static_cast<CameraRender::SkyBox*>(resource);
				DeleteSkybox(skybox->GetCubeMap());
				break;
			}
			default:
			{
				break;
			}
		}
	}

	void RHI::CreateShader(int& shaderProgram, std::vector<std::string> datas, std::vector<ShaderType> types)
	{
		int shader;
		std::vector<int> shaders;

		for (int i = 0; i < datas.size(); i++)
		{
			int success;
			const char* data = datas[i].c_str();

			shader = glCreateShader(types[i]);
			glShaderSource(shader, 1, &data, NULL);
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				std::string log(infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED" << log;
				shaderProgram = -1;
				return;
			}
			shaders.push_back(shader);
		}

		// link shaders
		shaderProgram = glCreateProgram();
		for (int i = 0; i < shaders.size(); i++)
			glAttachShader(shaderProgram, shaders[i]);
		glLinkProgram(shaderProgram);

		// check for linking errors
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::string log(infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << log;
			shaderProgram = -1;
			return;
		}

		// Delete shaders
		for (int i = 0; i < shaders.size(); i++)
			glDeleteShader(shaders[i]);
	}

	void RHI::UseShader(int shaderProgram)
	{
		glUseProgram(shaderProgram);
	}

	void RHI::DeleteShader(int shaderProgram)
	{
		glDeleteShader(shaderProgram);
	}

	void RHI::DeleteProgram(int shaderProgram)
	{
		glDeleteProgram(shaderProgram);
	}

	void RHI::DeleteFramebuffer(uint& frameBuffer)
	{
		glDeleteFramebuffers(1, &frameBuffer);
	}

	void RHI::DeleteBuffer(uint& frameBuffer)
	{
		glDeleteBuffers(1, &frameBuffer);
	}

	void RHI::DeleteRenderbuffer(uint& renderBuffer)
	{
		glDeleteRenderbuffers(1, &renderBuffer);
	}

	void RHI::DeleteVertexArray(uint& VAO)
	{
		glDeleteVertexArrays(1, &VAO);
	}

	void RHI::DeleteTexture(uint& texture)
	{
		glDeleteTextures(1, &texture);
	}

	void RHI::SetViewport(int right, int top, int width, int height)
	{
		glViewport(right, top, width, height);
	}

	void RHI::DrawMode(GProperties face, GProperties mode)
	{
		glPolygonMode(face, mode);
	}

	uint RHI::GetUniformLocation(Resources::Resource* shaderResource, std::string uniform)
	{
		Resources::Shader* shader = static_cast<Resources::Shader*>(shaderResource);

		if (shader->GetUniformLocations().find(uniform) != shader->GetUniformLocations().end())
			return shader->GetUniformLocations()[uniform];

		uint location = glGetUniformLocation(shader->GetShaderProgram(), uniform.c_str());
		shader->GetUniformLocations()[uniform] = location;
		return location;
	}

	void RHI::Send1iToShader(Resources::Resource* shader, const char* name, int nbr)
	{
		glUniform1i(GetUniformLocation(shader, name), nbr);
	}

	void RHI::Send1fToShader(Resources::Resource* shader, const char* name, float k1)
	{
		glUniform1f(GetUniformLocation(shader, name), k1);
	}

	void RHI::Send3fToShader(Resources::Resource* shader, const char* name, float k1, float k2, float k3)
	{
		glUniform3f(GetUniformLocation(shader, name), k1, k2, k3);
	}

	void RHI::Send3fToShader(Resources::Resource* shader, const char* name, Vector3D values)
	{
		glUniform3f(GetUniformLocation(shader, name), (float)values.x, (float)values.y, (float)values.z);
	}

	void RHI::Send4iToShader(Resources::Resource* shader, const char* name, Vector4D values)
	{
		glUniform4i(GetUniformLocation(shader, name), (int)values.x, (int)values.y, (int)values.z, (int)values.w);
	}

	void RHI::Send4fToShader(Resources::Resource* shader, const char* name, Vector4D values)
	{
		glUniform4f(GetUniformLocation(shader, name), (float)values.x, (float)values.y, (float)values.z, (float)values.w);
	}

	void RHI::SendMat4ToShader(Resources::Resource* shader, const char* name, GLsizei count, bool transpose, const float* value)
	{
		glUniformMatrix4fv(GetUniformLocation(shader, name), count, transpose, value);
	}

	void RHI::SetBackgroundType()
	{

	}

	void RHI::ClearBackground()
	{

	}

	void RHI::CreateTexture(unsigned char* data, int nrChannels, int width, int height, uint& texture, uint& sampler, bool gammaCorrection)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLenum internalFormat;
		GLenum dataFormat;

		if (nrChannels == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		if (nrChannels == 2)
		{
			internalFormat = GL_RG;
			dataFormat = GL_RG;
		}
		if (nrChannels == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//glGenSamplers(1, &sampler);
		//glBindSampler(texture, sampler);
		//glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);
		//glBindSampler(texture, 0);
	}

	void RHI::CreateNoiseTexture(uint &noiseTexture, std::vector<Vector3D> &ssaoNoise)
	{
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void RHI::CreateOneChannelFrameBuffer(uint& frameBuffer, uint& texture, int width, int height)
	{
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	}

	void RHI::CreateHDRFrameBuffer(uint& hdrFBO, uint& rboDepth, uint(&colorBuffers)[2], int width, int height)
	{
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

		glGenTextures(2, colorBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		}

		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RHI::CreatePingPongFrameBuffer(uint (&pingpongFBO)[2], uint(&pingpongBuffer)[2], int width, int height)
	{
		glGenFramebuffers(2, pingpongFBO);
		glGenTextures(2, pingpongBuffer);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
		}
	}

	void RHI::BindTexture2D(uint texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void RHI::DeleteTexture(uint& texture, uint& sampler)
	{
		glDeleteTextures(1, &texture);
		glDeleteSamplers(1, &sampler);
	}

	void RHI::DeleteSkybox(uint& cubemap)
	{
		glDeleteTextures(1, &cubemap);
	}

	void RHI::CreateModel(uint& VBO, uint& VAO, uint& EBO, std::vector<Vertex> vertexBuffer, std::vector<int> indexBuffer)
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		//glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), &vertexBuffer.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(unsigned int), indexBuffer.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::uv)));
		glEnableVertexAttribArray(2);

		// tangent attribute
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::tangent)));
		glEnableVertexAttribArray(3);

		// bitangent attribute
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::bitangent)));
		glEnableVertexAttribArray(4);

		// ids
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::boneIDs));
		glEnableVertexAttribArray(5);

		// weights
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::weights));
		glEnableVertexAttribArray(6);
	}

	void RHI::DeleteModel(uint& VBO, uint& VAO)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void RHI::SetCullFace(GProperties cullFace)
	{
		glCullFace(cullFace);
	}

	void RHI::SetDepthFunc(GProperties depth)
	{
		glDepthFunc(depth);
	}

	void RHI::SetBlendFunc(GProperties sFactor, GProperties dFactor)
	{
		glBlendFunc(sFactor, dFactor);
	}

	void RHI::SetPixelUnpackAlignment(int alignment)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	}

	void RHI::CreateFrameBuffer(uint& colorBufferCount)
	{
		glGenFramebuffers(1, &colorBufferCount);
	}

	void RHI::CreateRenderTexture(uint& indexBuffer, uint& indexTexture, uint width, uint height)
	{
		CreateFrameBuffer(indexBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, indexBuffer);

		glGenTextures(1, &indexTexture);
		glBindTexture(GL_TEXTURE_2D, indexTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, indexTexture, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																									 // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete no

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RHI::SetActiveFramebuffer(uint buffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	}

	void RHI::DeleteFramebuffers(sizei size, const uint* framebuffers)
	{
		glDeleteFramebuffers(size, framebuffers);
	}

	void RHI::Draw(uint modelIndex, sizei indexBufferSize)
	{
		glBindVertexArray(modelIndex);
		glDrawElements(GL_TRIANGLES, indexBufferSize, GL_UNSIGNED_INT, 0);
	}

	void RHI::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		ClearColorAndDepth();
	}

	void RHI::ClearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RHI::ClearColorAndDepth()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
	}

	void RHI::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void RHI::CreateGBuffer(uint& gBuffer, uint& gPosition, uint& gViewPosition, uint& gNormal, uint& gViewNormal, uint& gColor, uint& gMetallicRoughness, uint& depthBuffer, int width, int height)
	{
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		// position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

		glGenTextures(1, &gViewPosition);
		glBindTexture(GL_TEXTURE_2D, gViewPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gViewPosition, 0);

		// normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

		glGenTextures(1, &gViewNormal);
		glBindTexture(GL_TEXTURE_2D, gViewNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gViewNormal, 0);

		// color + specular color buffer
		glGenTextures(1, &gColor);
		glBindTexture(GL_TEXTURE_2D, gColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gColor, 0);

		// MetallicRougness
		glGenTextures(1, &gMetallicRoughness);
		glBindTexture(GL_TEXTURE_2D, gMetallicRoughness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gMetallicRoughness, 0);

		unsigned int attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};
		glDrawBuffers(6, attachments);

		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RHI::ActivateAndBindTexture2D(int nbTexture, uint texture)
	{
		glActiveTexture(GL_TEXTURE0 + nbTexture);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void RHI::ActivateAndBindCubemap(int nbTexture, uint cubemap)
	{
		glActiveTexture(GL_TEXTURE0 + nbTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	}

	void RHI::CreateScreenQuad(uint& quadVAO, uint& quadVBO)
	{
		float quadVertices[] =
		{
			// positions     // texture Coords
			-1.f,  1.f, 0.f, 0.f, 1.f,
			-1.f, -1.f, 0.f, 0.f, 0.f,
			 1.f,  1.f, 0.f, 1.f, 1.f,
			 1.f, -1.f, 0.f, 1.f, 0.f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	void RHI::CreateUIQuad(uint& UIVAO, uint& UIVBO)
	{
		glGenVertexArrays(1, &UIVAO);
		glGenBuffers(1, &UIVBO);
		glBindVertexArray(UIVAO);
		glBindBuffer(GL_ARRAY_BUFFER, UIVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RHI::CreateUIImageQuad(uint& UIImageVAO, uint& UIImageVBO)
	{
		float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(1, &UIImageVAO);
		glGenBuffers(1, &UIImageVBO);

		glBindBuffer(GL_ARRAY_BUFFER, UIImageVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(UIImageVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void RHI::DrawScreenQuad(uint quadVAO)
	{
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void RHI::CreateShadowFBOandMap(uint& shadowFBO, uint& shadowMap, Vector2D size)
	{
		glGenFramebuffers(1, &shadowFBO);
		glGenTextures(1, &shadowMap);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)size.x, (GLsizei)size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RHI::CreateShadowFBOandCubeMap(uint& shadowFBO, uint& shadowCubeMap, Vector2D size)
	{
		glGenFramebuffers(1, &shadowFBO);
		glGenTextures(1, &shadowCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);

		for (unsigned int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, (GLsizei)size.x, (GLsizei)size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RHI::CopyDepth(uint& fromBuffer, uint& toBuffer, const int width, const int height)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fromBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, toBuffer);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void RHI::CreateCubeMap(std::vector<Resources::Resource*> faces, unsigned int& cubeMap)
	{
		glGenTextures(1, &cubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		for (int i = 0; i < 6; i++)
		{
			Resources::Texture* face = static_cast<Resources::Texture*>(faces[i]);
			if (face != nullptr)
			{
				if (face->GetNrChannels() == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, face->GetWidth(), face->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, face->GetData());
				else if (face->GetNrChannels() == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, face->GetWidth(), face->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, face->GetData());
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void RHI::UpdateCubemapFace(int face, Resources::Resource* texture, unsigned int& cubeMap)
	{
		Resources::Texture* faceText = static_cast<Resources::Texture*>(texture);
		if (!faceText) return;

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB, faceText->GetWidth(), faceText->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, faceText->GetData());
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void RHI::CreateGlyph(uint& texture, int width, int height, unsigned char* data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	void WrapperRHI::LoadGLFunction()
	{
		gladLoadGL();
	}

	void RHI::ReadPixelColor(float x, float y, Vector4D& data)
	{
		unsigned char res[4];
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);
		data = Vector4D(res[0], res[1], res[2], res[3]);
	}
}