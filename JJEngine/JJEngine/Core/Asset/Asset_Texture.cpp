#include "Asset_Texture.h"
#include "Core/Graphics/Texture.h"
#include <Core/Utils/File.h>

bool Asset_Texture::CheckIsDataLoaded() const
{
	return data != nullptr;
}

bool Asset_Texture::LoadData()
{
	data = Texture::CreateTexture(File::ReadImageToTexture(path.string()));
	//fail check?
	return true;
}

void Asset_Texture::UnloadData()
{
	data = nullptr;
}
