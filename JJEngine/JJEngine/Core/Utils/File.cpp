#include "File.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stb_image.h>
#include "Assert.h"
#include "Log.h"
#include "Core/Graphics/Texture.h"

bool File::CheckExists(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        Log::Error("There is no file : {}", path);
        return false;
    }
    return true;
}

std::vector<std::filesystem::path> File::GetFileLists(const std::filesystem::path& dir_path)
{
    if (!CheckExists(dir_path))
        return {};

    std::vector<std::filesystem::path> file_lists;
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
    {
        file_lists.push_back(entry.path().string());
    }

    return file_lists;
}

std::vector<std::filesystem::path> File::GetFileListsRecv(const std::filesystem::path& dir_path)
{
    if (!CheckExists(dir_path))
        return {};

    std::vector<std::filesystem::path> file_lists;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path))
    {
        file_lists.push_back(entry.path().string());
    }

    return file_lists;
}

std::string File::ReadFileToString(const std::string& file_name)
{
    if (!CheckExists(file_name)) return {};

    std::ifstream file(file_name);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::tuple<std::shared_ptr<char[]>, int> File::ReadFileToBytes(const std::filesystem::path& file_name)
{
    if (!CheckExists(file_name)) return {};

    std::ifstream stream(file_name, std::ios::binary | std::ios::ate);

    if (!stream)
    {
        // Failed to open the file
        return { nullptr, 0 };
    }

    std::streampos end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    int size = static_cast<int>(end - stream.tellg());

    if (size == 0)
    {
        // File is empty
        return { nullptr, 0 };
    }

    std::shared_ptr<char[]> buffer(new char[size]);
    stream.read(buffer.get(), size);
    stream.close();

    return { buffer, size };
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

std::shared_ptr<TextureData> File::ReadHDRImageToTexture(const std::string& file_name)
{
    if (!std::filesystem::exists(file_name))
    {
        Log::Error("There is no file : {}", file_name);
        return {};
    }
    std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();
    stbi_set_flip_vertically_on_load(true);
    int number_of_channels = 0;
    float* img = stbi_loadf(file_name.c_str(), &(texture->width), &(texture->height), &number_of_channels, 0);
    if (img == nullptr)
    {
        std::cout << "error - on loading texture" << std::endl;
    }
    switch (number_of_channels)
    {
    case 3:
        texture->channel = TextureChannel::RGB16F;
        break;
    case 4:
        texture->channel = TextureChannel::RGBA32F;
        break;
    default:
        ENGINE_ASSERT(false, "Not Supported Channel");
        break;
    }
    std::shared_ptr<float[]> data{ img, [](float* to_delete) { stbi_image_free(to_delete); } };
    texture->data = data;
    return texture;
}

std::shared_ptr<TextureData> File::ReadCubeMapImagesToTexture(const std::vector<std::string>& file_names)
{
    ENGINE_ASSERT(file_names.size() == 6, "There should be 6 faces");
    std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();
    texture->target = TextureTarget::CubeMap;
    texture->channel = TextureChannel::RGB16F;

    stbi_set_flip_vertically_on_load(false);

    std::shared_ptr<float* []> datas{new float* [6],
    [](float** imgs_to_delete)
    {
        for (int i = 0; i < 6; ++i)
        {
            stbi_image_free(imgs_to_delete[i]);
        }
    	delete[] imgs_to_delete;
    }};

    for (int i = 0; i < 6; ++i)
    {
        if (!std::filesystem::exists(file_names[i]))
        {
            EngineLog::Error("There is no file : {}", file_names[i]);
            return {};
        }

        datas[i] = stbi_loadf(file_names[i].c_str(), &(texture->width), &(texture->height), nullptr, 3);
        if (datas[i] == nullptr)
        {
            EngineLog::Error("Error - On loading texture : {}", file_names[i]);
        }
    }
    texture->data = datas;
    return texture;
}
