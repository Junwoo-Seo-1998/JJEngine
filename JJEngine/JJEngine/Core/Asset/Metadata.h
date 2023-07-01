#pragma once
#include "AssetType.h"
#include "uuid.h"
#include <filesystem>

using AssetHandle = uuids::uuid;

struct Metadata {
	AssetHandle Handle;
	std::filesystem::path path;
	AssetType type;
	bool isMemoryOnlyAsset{false};
};