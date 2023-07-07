#include "GameDataSerializer.h"
#include "Core/Utils/YAML_IMPL.hpp"
#include <fstream>
#include "Core/Utils/Assert.h"
#include "Core/Application.h"
#include "Core/Asset/Manager/AssetManager.h"

#define GAME_DATA "GAME_DATA"
#define START_SCENE "StartScene"

GameDataSerializer::GameDataSerializer(std::string game_data)
{
	std::ifstream file{ game_data };
	std::stringstream strStream{};
	strStream << file.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	ASSERT(data[GAME_DATA], "File is not a GameData");

	startScene = data[START_SCENE].as<AssetHandle>();

	file.close();
}

std::shared_ptr<Asset_Scene> GameDataSerializer::GetStartScene()
{
	return Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(startScene);
}
