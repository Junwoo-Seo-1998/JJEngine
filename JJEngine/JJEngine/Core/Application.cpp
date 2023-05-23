/* Start Header -------------------------------------------------------
Project: JJEngine 
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#include <chrono>
#include "Application.h"

#include "glad.h"
#include "Window.h"
#include "Input/Input.h"
#include "SceneManager.h"
#include "Time.h"
#include "ImGui/ImGuiRenderer.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerManager.h"
#include "Script/ScriptEngine.h"
#include "Utils/Assert.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(s_Instance == nullptr, "You are making Application Twice!!");
	s_Instance = this;
	window = std::make_shared<Window>();
	sceneManager = std::make_shared<SceneManager>();
	layerManager = std::make_shared<LayerManager>();
}

Application::~Application()
{
}

bool Application::Init()
{
	ImGuiRenderer::Instance()->Init(GetWindow()->GetGLFWWindow());
	ScriptEngine::instance()->Init();
	return true;//just for now
}

void Application::Update()
{
	bool engineLoop{ true };
	do
	{
		Time::Update();
		if (window->shouldClose() == true) {
			sceneManager->exit();
			engineLoop = false;
		}

		window->update([&]()
		{
			auto& overlays = layerManager->GetOverLays();
			auto& layers = layerManager->GetLayers();

			//imgui
			ImGuiRenderer::Instance()->GuiBegin();

			sceneManager->update();// this will be moved up later

			{
				for (auto layer : layers)
					layer->OnUpdate();
				for (auto layer : overlays)
					layer->OnUpdate();
			}

			{
				for (auto layer : layers)
					layer->OnPreRender();
				for (auto layer : overlays)
					layer->OnPreRender();
			}

			{
				for (auto layer : layers)
					layer->OnRender();
				for (auto layer : overlays)
					layer->OnRender();
			}

			{
				for (auto layer : layers)
					layer->OnPostRender();
				for (auto layer : overlays)
					layer->OnPostRender();
			}

			{
				for (auto layer : layers)
					layer->OnImGuiRender();
				for (auto layer : overlays)
					layer->OnImGuiRender();
			}
			ImGuiRenderer::Instance()->GuiEnd();
		});
		layerManager->ClearDeleteQueue();
	} while (engineLoop);
}

void Application::Shutdown()
{
	ScriptEngine::instance()->Shutdown();
	ImGuiRenderer::Instance()->Shutdown();
}

std::shared_ptr<SceneManager> Application::GetSceneManager()
{
	return sceneManager;
}

std::shared_ptr<Window> Application::GetWindow()
{
	return window;
}

std::shared_ptr<LayerManager> Application::GetLayerManager()
{
	return layerManager;
}
