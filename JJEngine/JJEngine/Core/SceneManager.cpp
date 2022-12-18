/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#include <iostream>
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::enrollScene(int i, Scene& scene)
{
	scenes[i] = &scene;
}

void SceneManager::enrollScene(int /*i*/, std::string /*path*/)
{
	//will be implemented
}

void SceneManager::setNextScene(int i)
{
	if (scenes.find(i) == scenes.end()) {
		std::cerr << "No "<< i<<"th scene exist" << std::endl;
		return;
	}
	nextScene = scenes[i];
}

void SceneManager::returnPrevScene()
{
	nextScene = prevScene;
}

void SceneManager::Init()
{
}

void SceneManager::update(double dt)
{
	switch (nowState)
	{
	case SceneManager::SceneManagerState::start:
		if (nextScene == nullptr) {
			nowState = SceneManagerState::Exit;
			std::cerr << "No scene enrolled" << std::endl;
		}
		else
		{
			nowState = SceneManagerState::Load;
		}
		break;
	case SceneManager::SceneManagerState::Load:
		prevScene = nowScene;
		nowScene = nextScene;
		nowScene->Load();
		nowState = SceneManagerState::Update;
		break;
	case SceneManager::SceneManagerState::Update:
		if (nowScene != nextScene) {
			nowState = SceneManagerState::Unload;
		}
		else {
			nowScene->Update(dt);
			nowScene->Draw();
		}
		break;
	case SceneManager::SceneManagerState::Unload:
		nowState = SceneManagerState::Load;
		nowScene->Unload();
		break;
	case SceneManager::SceneManagerState::Exit:
		if (nowScene != nullptr) { nowScene->Unload(); }
		break;
	default:
		break;
	}
}

void SceneManager::exit() {
	nowState = SceneManagerState::Exit;
}
