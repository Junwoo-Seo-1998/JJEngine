#pragma once
#include "Core/Scene.h"
#include <string>
#include <iostream>

class TestScene: public Scene
{
	std::string text{};
public:
	TestScene(std::string);
	~TestScene();
	void Load() override{ std::cout << text << std::endl; };
	void Update(double) override {}
	void Draw() override {}
	void Unload()  override {}
private:

};

TestScene::TestScene(std::string t):Scene(),text(t)
{
}

TestScene::~TestScene()
{
}