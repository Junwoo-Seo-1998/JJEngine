#include "File.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_image.h>
#include <format>
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
    unsigned char* img = stbi_load(file_name.c_str(), &(texture->width), &(texture->height), &(texture->number_of_channels), 3);
    if (img == nullptr)
    {
        std::cout << "error - on loading texture" << std::endl;
    }
    std::shared_ptr<unsigned char[]> data{ img, [](unsigned char* to_delete) { stbi_image_free(to_delete); } };
    texture->data = data;
    return texture;
}
