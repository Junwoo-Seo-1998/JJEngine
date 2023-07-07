/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <string>

class Event;
class EventManager;
class Application;

class Window;
class SceneManager;
class LayerManager;
class AssetManager;
class Application
{
protected:
	Application();
public:
	virtual ~Application();

	bool Init();
	void Update();
	void Shutdown();

	void OnEvent(Event& event);

	std::shared_ptr<SceneManager> GetSceneManager();
	std::shared_ptr<Window> GetWindow();
	std::shared_ptr<LayerManager> GetLayerManager();
	std::shared_ptr<EventManager> GetEventManager();
	std::shared_ptr<AssetManager> GetAssetManager();
	virtual void UserDefinedInit(std::string Data);


	static inline Application& Instance() { return *s_Instance; }
private:
	static Application* s_Instance;

	std::shared_ptr<SceneManager> sceneManager;
	std::shared_ptr<Window> window;
	std::shared_ptr<LayerManager> layerManager;
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<AssetManager> assetManager;
};
