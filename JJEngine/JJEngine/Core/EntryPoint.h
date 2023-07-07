#pragma once
#include"Core/Application.h"
#include<memory>
#include<iostream>
#include "Utils/Log.h"

extern std::shared_ptr<Application> CoreMain();
int main(int argc, char** argv)
{
	std::shared_ptr<Application> application = CoreMain();
	EngineLog::Info("Engine Init!");
	if (!application->Init())
	{
		std::cout << "error on init!" << std::endl;
		return -1;
	}
	{
		std::string GameData{};
		if (argc > 1) GameData = argv[1]; 
		application->UserDefinedInit(GameData);
	}
	application->Update();
	application->Shutdown();
	return 0;
}
