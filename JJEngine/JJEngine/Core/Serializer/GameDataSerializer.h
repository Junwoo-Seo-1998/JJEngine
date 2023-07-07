#pragma once
#include <string>
#include <memory>
#include <uuid.h>

using AssetHandle = uuids::uuid;

class Asset_Scene;
class GameDataSerializer {
	AssetHandle startScene{};
public:
	GameDataSerializer(std::string game_data);

	std::shared_ptr<Asset_Scene> GetStartScene();
};