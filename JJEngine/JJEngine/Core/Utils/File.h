#pragma once
#include <string>
#include <memory>
#include <filesystem>
struct TextureData;
class File
{
public:
	static bool CheckExists(const std::string& path);
	static std::vector<std::string> GetFileLists(const std::string& dir_path);
	static std::string ReadFileToString(const std::string& file_name);
	static std::shared_ptr<TextureData> ReadImageToTexture(const std::string& file_name);
};
