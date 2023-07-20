#include "Asset_HDRTexture.h"
#include "Core/Graphics/Texture.h"
#include <Core/Utils/File.h>

bool Asset_HDRTexture::CheckIsDataLoaded() const
{
	return data != nullptr;
}

bool Asset_HDRTexture::LoadData(std::filesystem::path path)
{
	data = Texture::CreateTexture(File::ReadHDRImageToTexture(path.string()));
	//fail check?
	return true;
}

void Asset_HDRTexture::UnloadData()
{
	data = nullptr;
}
