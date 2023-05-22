#include "Core/EntryPoint.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "ShadowScene.h"

#include "Core/Utils/Log.h"
#include "FileWatch.hpp"
#include <filesystem>

class GameApp : public Application
{
	ShadowScene shadowScene;

	virtual void UserDefinedInit() override
	{
		GetSceneManager()->enrollScene(0, shadowScene);
		GetSceneManager()->setNextScene(0);
		//todo: remove this
		filewatch::FileWatch<std::string> watch("./Resources/Scripts/JJEngine-ScriptCore.dll", [](const std::string& path, const filewatch::Event change_type) {
			switch (change_type)
			{
			case filewatch::Event::added:
				std::cout << "The file was added to the directory." << '\n';
				break;
			case filewatch::Event::removed:
				std::cout << "The file was removed from the directory." << '\n';
				break;
			case filewatch::Event::modified:
				std::cout << "The file was modified. This can be a change in the time stamp or attributes." << '\n';
				break;
			case filewatch::Event::renamed_old:
				std::cout << "The file was renamed and this is the old name." << '\n';
				break;
			case filewatch::Event::renamed_new:
				std::cout << "The file was renamed and this is the new name." << '\n';
				break;
			}
		});

	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<GameApp>();
}
