#pragma once

using AssetHandle = uuids::uuid;

class Mesh;
struct MeshComponent
{
	//todo : will be replaced with mesh asset handle
	AssetHandle handle{};
};
