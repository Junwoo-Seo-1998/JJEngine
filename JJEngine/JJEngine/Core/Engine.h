/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
class Window;
class SceneManager;
class JJEngine
{
	JJEngine();

	SceneManager* sceneManager{};
	Window* window{};

public:
	~JJEngine();
	static std::shared_ptr<JJEngine> instance();
	static void EngineOn();
	static void update();
	static void EngineOff();

	static SceneManager* GetSceneManager();
	static Window* GetWindow();
};

namespace JJ {
	
}