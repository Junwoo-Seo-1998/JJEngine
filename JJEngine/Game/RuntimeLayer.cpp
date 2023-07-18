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
#include <Core/Event/Event.h>

#include "Core/Event/SceneEvent.h"

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
	m_ActiveScene->ResizeViewport(width, height);
	m_SceneRenderer->SetViewportSize(width, height);
}

void RuntimeLayer::OnPreRender()
{
}

void RuntimeLayer::OnRender()
{
	m_ActiveScene->UpdateRuntime(m_SceneRenderer);
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
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<LoadSceneEvent>([&](LoadSceneEvent& e)
	{
			//std::shared_ptr<Asset_Scene> temp = Application::Instance().GetAssetManager()->GetEnrolledScene(e.GetSceneName());
			AssetHandle handle{ Application::Instance().GetAssetManager()->GetHandleFromPath(e.GetSceneName()) };
			std::shared_ptr<Asset_Scene> temp = Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(handle);
			if (temp == nullptr) return false;
			SetNewScene(temp);
			return true;
	});
}

void RuntimeLayer::SetNewScene(std::shared_ptr<Asset_Scene> new_scene)
{
	if (m_ActiveScene != nullptr) m_ActiveScene->StopRuntime();
	m_ActiveScene = Scene::Copy(new_scene->data);
	m_ActiveScene->Awake();
	m_ActiveScene->Start();
	m_ActiveScene->StartRuntime();
}
