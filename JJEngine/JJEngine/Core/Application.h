/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>

class Application;

class Window;
class SceneManager;
class LayerManager;
class Application
{
protected:
	Application();
public:
	virtual ~Application();

	bool Init();
	void Update();
	void Shutdown();
	std::shared_ptr<SceneManager> GetSceneManager();
	std::shared_ptr<Window> GetWindow();
	std::shared_ptr<LayerManager> GetLayerManager();
	virtual void UserDefinedInit() {};


	static inline Application& Instance() { return *s_Instance; }
private:
	static Application* s_Instance;

	std::shared_ptr<SceneManager> sceneManager;
	std::shared_ptr<Window> window;
	std::shared_ptr<LayerManager> layerManager;
};
