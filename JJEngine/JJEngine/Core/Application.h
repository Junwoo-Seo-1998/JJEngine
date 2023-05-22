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
class Application
{
protected:
	Application();
public:
	virtual ~Application();

	bool Init();
	void Update();
	void Shutdown();
	SceneManager* GetSceneManager();
	Window* GetWindow();

	virtual void UserDefinedInit() {};
private:
	SceneManager* sceneManager{};
	Window* window{};
};
