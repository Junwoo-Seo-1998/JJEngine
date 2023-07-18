#include "GameDataSerializer.h"
#include "Core/Utils/YAML_IMPL.hpp"
#include <fstream>
#include "Core/Utils/Assert.h"
#include "Core/Application.h"
#include "Core/Asset/Manager/AssetManager.h"

#define GAME_DATA "GAME_DATA"
#define START_SCENE "StartScene"
#define SCENE_LIST "Scenes"

GameDataSerializer::GameDataSerializer(std::string game_data):path(game_data)
{
}

void GameDataSerializer::Serialize()
{
	std::ifstream file{ path };
	std::stringstream strStream{};
	strStream << file.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	ASSERT(data[GAME_DATA], "File is not a GameData");

	startScene = data[START_SCENE].as<AssetHandle>();

	//YAML::Node scene_list = data[SCENE_LIST];
	//for (auto d : scene_list) {
	//	projectScenes[d.first.as<std::string>()] = d.second.as<AssetHandle>();
	//}

	file.close();
}

void GameDataSerializer::Deserialize()
{
}

std::shared_ptr<Asset_Scene> GameDataSerializer::GetStartScene()
{
	return Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(startScene);
}

std::map<AssetHandle, std::string> GameDataSerializer::GetProjectScenes()
{
	return projectScenes;
}
