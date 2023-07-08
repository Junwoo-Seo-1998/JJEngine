#include "Asset_Mesh.h"

bool Asset_Mesh::CheckIsDataLoaded() const
{
	return data != nullptr;
}

bool Asset_Mesh::LoadData(std::filesystem::path p)
{
	return false;
}

void Asset_Mesh::UnloadData()
{
	data = nullptr;
}
