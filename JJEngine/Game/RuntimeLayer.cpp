#include "RuntimeLayer.h"
#include <Core/Scene.h>
#include <Core/Graphics/Renderer/SceneRenderer.h>
#include <Core/Application.h>
#include <Core/Asset/Manager/AssetManager.h>
RuntimeLayer::~RuntimeLayer()
{
}

void RuntimeLayer::OnAttach()
{
}

void RuntimeLayer::OnDetach()
{
}

void RuntimeLayer::OnStart()
{
	m_SceneRenderer = std::make_shared<SceneRenderer>();
	AssetHandle handle = Application::Instance().GetAssetManager()->GetHandleFromPath("./Resources/Scenes/Test.scn");
	m_ActiveScene = Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(handle);
	m_ActiveScene->data->Awake();
	m_ActiveScene->data->Start();
	m_ActiveScene->data->StartRuntime();
}

void RuntimeLayer::OnUpdate()
{
}

void RuntimeLayer::OnPreRender()
{
}

void RuntimeLayer::OnRender()
{
	m_ActiveScene->data->UpdateRuntime(m_SceneRenderer);
}

void RuntimeLayer::OnPostRender()
{
}

void RuntimeLayer::OnImGuiRender()
{
}

void RuntimeLayer::OnEvent(Event& event)
{
}
