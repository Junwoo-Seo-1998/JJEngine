#pragma once
#include <string>
#include <memory>
#include <uuid.h>
#include <map>

using AssetHandle = uuids::uuid;

class Asset_Scene;
class GameDataSerializer {
	std::string path{};
public:
	GameDataSerializer(std::string game_data);

	void Serialize();
	void Deserialize();

	AssetHandle startScene{};
	std::map<AssetHandle, std::string> projectScenes{};
	std::shared_ptr<Asset_Scene> GetStartScene();
	std::map<AssetHandle, std::string> GetProjectScenes();
};