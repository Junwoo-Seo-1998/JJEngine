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

	YAML::Node scene_list = data[SCENE_LIST];
	for (auto d : scene_list) {
		projectScenes[d.first.as<AssetHandle>()] = d.second.as<std::string>();
	}

	file.close();
}

void GameDataSerializer::Deserialize()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << GAME_DATA;
	out << YAML::Value << GAME_DATA;
	out << YAML::Key << START_SCENE;
	out << YAML::Value << startScene;

	out << YAML::Key << SCENE_LIST;
	out << YAML::Value << YAML::BeginMap;
	for (auto& s:projectScenes) {
		out << YAML::Key << s.first;
		out << YAML::Value << s.second;
	}
	out << YAML::EndMap;
	
	out << YAML::EndMap;



	std::ofstream file{path};
	file << out.c_str();
	file.close();
}

std::shared_ptr<Asset_Scene> GameDataSerializer::GetStartScene()
{
	return Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Scene>(startScene);
}

std::map<AssetHandle, std::string> GameDataSerializer::GetProjectScenes()
{
	return projectScenes;
}
