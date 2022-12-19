/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
class JJEngine;
namespace statics
{
	std::shared_ptr<JJEngine> createInstance();
}

class Window;
class SceneManager;
class JJEngine
{
	friend std::shared_ptr<JJEngine> statics::createInstance();
	JJEngine();

	SceneManager* sceneManager{};
	Window* window{};

public:
	~JJEngine();
	static std::shared_ptr<JJEngine> instance();
	static void init();
	static void update();
	static void shutdown();
	static SceneManager* GetSceneManager();
	static Window* GetWindow();
};

namespace JJ {
	
}