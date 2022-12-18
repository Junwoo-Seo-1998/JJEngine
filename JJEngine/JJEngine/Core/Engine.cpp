/* Start Header -------------------------------------------------------
Project: JJEngine 
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#include <chrono>
#include "Engine.h"
#include "Window.h"
#include "SceneManager.h"
namespace statics
{
	std::shared_ptr<JJEngine> instance = nullptr;
}
JJEngine::JJEngine()
{
	window = new Window{};
	sceneManager = new SceneManager{};
}

JJEngine::~JJEngine()
{
	delete window;
	delete sceneManager;
}

std::shared_ptr<JJEngine> JJEngine::instance()
{
	return statics::instance;
}

void JJEngine::EngineOn()
{
	if (statics::instance == nullptr) statics::instance = std::make_shared<JJEngine>(*(new JJEngine{}));
}

void JJEngine::update()
{
	bool engineLoop{ true };
	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	double dt{ };
	do
	{
		const std::chrono::time_point now{ std::chrono::system_clock::now() };
		dt = { std::chrono::duration<double>(now - lastTick).count() };
		if (instance()->window->shouldClose() == true) {
			instance()->sceneManager->exit();
			engineLoop = false;
		}
		instance()->sceneManager->update(dt);
		instance()->window->update();

		lastTick = now;
	} while (engineLoop);
}

void JJEngine::EngineOff()
{
	statics::instance = nullptr;
}

SceneManager* JJEngine::GetSceneManager()
{
	return instance()->sceneManager;
}

Window* JJEngine::GetWindow()
{
	return instance()->window;
}