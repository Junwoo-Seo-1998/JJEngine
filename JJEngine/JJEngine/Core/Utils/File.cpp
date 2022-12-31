#include "File.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_image.h>
#include "Assert.h"
#include "Log.h"
#include "Core/Graphics/Texture.h"
std::string File::ReadFileToString(const std::string& file_name)
{
    if(!std::filesystem::exists(file_name))
    {
        Log::Error("There is no file : {}", file_name);
        return {};
    }
    std::ifstream file(file_name);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::shared_ptr<TextureData> File::ReadImageToTexture(const std::string& file_name)
{
    if (!std::filesystem::exists(file_name))
    {
        Log::Error("There is no file : {}", file_name);
        return {};
    }
    std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();
    stbi_set_flip_vertically_on_load(true);
    int number_of_channels = 0;
    unsigned char* img = stbi_load(file_name.c_str(), &(texture->width), &(texture->height), &number_of_channels, 0);
    if (img == nullptr)
    {
        std::cout << "error - on loading texture" << std::endl;
    }
    switch (number_of_channels)
    {
    case 3:
        texture->channel = TextureChannel::RGB;
        break;
    case 4:
        texture->channel = TextureChannel::RGBA;
        break;
    default:
        ENGINE_ASSERT(false, "Not Supported Channel");
        break;
    }
    std::shared_ptr<unsigned char[]> data{ img, [](unsigned char* to_delete) { stbi_image_free(to_delete); } };
    texture->data = data;
    return texture;
}
