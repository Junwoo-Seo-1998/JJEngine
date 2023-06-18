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
#include "Event/EventManager.h"
#include "Time.h"
#include "ImGui/ImGuiRenderer.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerManager.h"
#include "Script/ScriptEngine.h"
#include "Utils/Assert.h"
#include "Core/Asset/Manager/AssetManager.h"

#include "Graphics/Renderer/Renderer2D.h"
Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(s_Instance == nullptr, "You are making Application Twice!!");
	s_Instance = this;
	eventManager = std::make_shared<EventManager>();
	window = std::make_shared<Window>();
	sceneManager = std::make_shared<SceneManager>();
	layerManager = std::make_shared<LayerManager>();
	assetManager = std::make_shared<AssetManager>();
}

Application::~Application()
{
}

bool Application::Init()
{
	Renderer2D::Init();
	ImGuiRenderer::Instance()->Init(GetWindow()->GetGLFWWindow());
	Script::ScriptEngine::Init();
	if (assetManager->ReadAData() == false) {}

	return true;//just for now
}

void Application::Update()
{
	{//layer start
		auto& overlays = layerManager->GetOverLays();
		auto& layers = layerManager->GetLayers();
		{
			for (auto layer : layers)
				layer->OnStart();
			for (auto layer : overlays)
				layer->OnStart();
		}
	}

	bool engineLoop{ true };
	do
	{
		Time::Update();
		assetManager->UpdateAData();

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

			std::shared_ptr<Event> event = eventManager->PollEvent();
			while (event)
			{
				{//dispatch event
					for (auto layer : overlays)
						layer->OnEvent(*event);
					for (auto layer : layers)
						layer->OnEvent(*event);
				}
				event = eventManager->PollEvent();
			}
			

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
	Script::ScriptEngine::Shutdown();
	Renderer2D::Shutdown();
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

std::shared_ptr<EventManager> Application::GetEventManager()
{
	return eventManager;
}

std::shared_ptr<AssetManager> Application::GetAssetManager()
{
	return assetManager;
}
