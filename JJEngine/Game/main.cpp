#include "Core/EntryPoint.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "TestScene.h"
#include "ShadowScene.h"

#include "Core/Utils/Log.h"
#include "FileWatch.hpp"
#include <filesystem>

class MyApp : public Application
{
	TestScene test1{ " test 1 " };
	TestScene test2{ " test 2 " };
	ShadowScene shadowScene;

	virtual void UserDefinedInit() override
	{
		//Application::GetSceneManager()->enrollScene(0, test1);
		//Application::GetSceneManager()->enrollScene(1, test2);
		Application::GetSceneManager()->enrollScene(2, shadowScene);
		Application::GetSceneManager()->setNextScene(2);


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
				//JJEngine::ReloadScript();
				break;
			case filewatch::Event::renamed_old:
				std::cout << "The file was renamed and this is the old name." << '\n';
				break;
			case filewatch::Event::renamed_new:
				std::cout << "The file was renamed and this is the new name." << '\n';
				break;
			}

		//JJEngine::ReloadScript();
			});

	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<MyApp>();
}
