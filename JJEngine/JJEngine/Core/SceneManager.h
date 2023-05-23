/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#pragma once
#include <map>
#include <string>


class Scene;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	std::shared_ptr<Scene> GetCurrentScene();

	void enrollScene(int i, std::shared_ptr<Scene> scene);
	void enrollScene(int i, std::string path);
	void setNextScene(int i);
	void returnPrevScene();
	void Init();
	void update();
	void exit();

	
private:
	enum class SceneManagerState {
		start,
		Load,
		Update,
		Unload,
		Exit,
	};

	std::shared_ptr<Scene> nowScene{};
	std::shared_ptr<Scene> nextScene{};
	std::shared_ptr<Scene> prevScene{};
	SceneManagerState nowState{};

	std::map<int, std::shared_ptr<Scene>> scenes{};
};

