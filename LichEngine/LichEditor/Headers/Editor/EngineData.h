#pragma once
#include <Scene/SceneManager.h>
#include <Resources/ResourceManager.h>
struct EngineData
{
	SceneManagement::SceneManager* pSceneManager;
	Renderer::RenderManager* pRenderManager;
	Resources::ResourceManager* pResouceManager;
};