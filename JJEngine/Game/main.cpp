#include "Core/test.h"
#include "Core/Engine.h"
#include "Core/SceneManager.h"
#include "TestScene.h"
#include "Core/Utils/Log.h"
#include "FileWatch.hpp"
#include <filesystem>



int main(void)
{
	EngineLog::Info("Engine Init!");
	JJEngine::init();
	TestScene test1{" test 1 "};
	TestScene test2{" test 2 "};

	filewatch::FileWatch<std::string> watch("./Resources/Scripts/JJEngine-ScriptCore.dll", [](const std::string & path, const filewatch::Event change_type) {
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
	/*auto lists = File::GetFileLists("./");
	for (auto& str : lists)
	{
		std::cout << str << std::endl;
	}*/
	JJEngine::GetSceneManager()->enrollScene(0, test1);
	JJEngine::GetSceneManager()->enrollScene(1, test2);
	JJEngine::GetSceneManager()->setNextScene(1);
	Log::Warn("Warn");
	Log::Info("Info");
	Log::Trace("Trace");
	Log::Critical("Critical");
	Log::Debug("Debug");
	Log::Error("Error");
	Log::Error("Some error message.. with format {} ,{}", 1, 3.23);
	JJEngine::update();
	JJEngine::shutdown();
	hello();
	return 0;
}
