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
	void enrollScene(int i, Scene& scene);
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

	Scene* nowScene{};
	Scene* nextScene{};
	Scene* prevScene{};
	SceneManagerState nowState{};

	std::map<int, Scene*> scenes{};
};

