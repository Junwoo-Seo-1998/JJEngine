#include "Core/EntryPoint.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "ShadowScene.h"
#include "Core/Utils/Log.h"
#include "FileWatch.hpp"
#include "Core/Layer/LayerManager.h"
//#include "IngameCameraControllerLayer.h"
//#include "IngameRendererLayer.h"
#include "RuntimeLayer.h"

#include <filesystem>

class GameApp : public Application
{
	virtual void UserDefinedInit(std::string GameData) override
	{
		Application::UserDefinedInit(GameData);
		GetLayerManager()->PushLayer(std::make_shared<RuntimeLayer>());
		//GetLayerManager()->PushLayer(std::make_shared<IngameCameraControllerLayer>());
		//GetLayerManager()->PushLayer(std::make_shared<IngameRendererLayer>());

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
