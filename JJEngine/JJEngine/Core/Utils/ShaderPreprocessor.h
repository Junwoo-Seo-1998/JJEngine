#pragma once
#include <filesystem>
class ShaderPreprocesssor
{
public:
	static void Preprocess(const std::filesystem::path& file);
private:
	static std::string CopyWithoutComments(const std::string& sourceString);
};