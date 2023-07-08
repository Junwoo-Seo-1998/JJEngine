#include "RuntimeLayer.h"
#include <Core/Scene.h>
#include <Core/Graphics/Renderer/SceneRenderer.h>
#include <Core/Application.h>
#include <Core/Asset/Manager/AssetManager.h>
#include <Core/Graphics/RenderPass.h>
#include <Core/Graphics/Renderer/Renderer.h>

#include <Core/Window.h>
#include <Core/Graphics/FrameBuffer.h>
#include <Core/Scene.h>

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
	SetNewScene(Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(handle));
}

void RuntimeLayer::OnUpdate()
{
	auto [width, height] = Application::Instance().GetWindow()->GetWidthAndHeight();
	//if you want to support dynamic viewport uncomment it
	//m_ActiveScene->data->ResizeViewport(width, height);
	m_SceneRenderer->SetViewportSize(width, height);
}

void RuntimeLayer::OnPreRender()
{
}

void RuntimeLayer::OnRender()
{
	m_ActiveScene->data->UpdateRuntime(m_SceneRenderer);
	//draw final image
	Renderer::DrawFinalFullScreenQuad(m_SceneRenderer->GetFinalRenderPass()->GetSpecification().TargetFramebuffer->GetColorTexture(0));
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

void RuntimeLayer::SetNewScene(std::shared_ptr<Asset_Scene> new_scene)
{
	m_ActiveScene = new_scene;
	m_ActiveScene->data->Awake();
	m_ActiveScene->data->Start();
	m_ActiveScene->data->StartRuntime();
}
