#pragma once
#include <string>
#include <memory>
#include <tuple>
#include <filesystem>
struct TextureData;
class File
{
public:
	static bool CheckExists(const std::filesystem::path& path);
	static std::vector<std::filesystem::path> GetFileLists(const std::filesystem::path& dir_path);
	static std::vector<std::filesystem::path> GetFileListsRecv(const std::filesystem::path& dir_path);
	static std::string ReadFileToString(const std::string& file_name);
	static std::tuple<std::shared_ptr<char[]>, int> ReadFileToBytes(const std::filesystem::path& file_name);
	static std::shared_ptr<TextureData> ReadImageToTexture(const std::string& file_name);
};
