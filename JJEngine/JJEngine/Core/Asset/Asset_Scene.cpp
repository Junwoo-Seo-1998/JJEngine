#include "Asset_Scene.h"
#include "Core/Scene.h"
#include <Core/SceneSerializer.h>
#include <Core/Utils/Log.h>
bool Asset_Scene::CheckIsDataLoaded() const
{
	return data != nullptr;
}

bool Asset_Scene::LoadData()
{
	data = std::make_shared<Scene>(path.filename().string());
	SceneSerializer see_real(data);
	data->SetScenePath(path);
	if (see_real.Deserialize(path.string()) == false) {
		Log::Error("Fail to deserialize Scene: " + path.filename().string());
		data = nullptr;
		return false;
	}
	return true;
}

void Asset_Scene::UnloadData()
{
	data = nullptr;
}
