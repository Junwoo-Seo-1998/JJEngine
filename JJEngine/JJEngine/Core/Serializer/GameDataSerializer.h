#pragma once
#include <string>
#include <memory>
#include <uuid.h>
#include <map>

using AssetHandle = uuids::uuid;

class Asset_Scene;
class GameDataSerializer {
	AssetHandle startScene{};
	std::map<std::string, AssetHandle> projectScenes{};
public:
	GameDataSerializer(std::string game_data);

	std::shared_ptr<Asset_Scene> GetStartScene();
	std::map<std::string, AssetHandle> GetProjectScenes();
};