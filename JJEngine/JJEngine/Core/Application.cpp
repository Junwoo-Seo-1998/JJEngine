/* Start Header -------------------------------------------------------
Project: JJEngine 
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#include <chrono>
#include "Application.h"
#include "Window.h"
#include "Input/Input.h"
#include "SceneManager.h"
#include "Time.h"
#include "ImGui/ImGuiRenderer.h"
#include "Script/ScriptEngine.h"
#include "Utils/Assert.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(s_Instance == nullptr, "You are making Application Twice!!");
	s_Instance = this;
	window = std::make_shared<Window>();
	sceneManager = std::make_shared<SceneManager>();
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
			ImGuiRenderer::Instance()->GuiBegin();
			sceneManager->update();
			ImGuiRenderer::Instance()->GuiEnd();
		});
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