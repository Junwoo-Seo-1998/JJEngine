#pragma once
#include <filesystem>
#include <unordered_map>
#include "Core/Graphics/Shader.h"

class ShaderPreprocesssor
{
	
public:
	static void Preprocess(const std::filesystem::path& file);
private:
	static std::string CopyWithoutComments(const std::string& sourceString);
	static void Testing(std::string& sourceString);
	static void CommandProcessing(std::unordered_map<ShaderType, std::string>& shaders, ShaderType current_shader_type, std::string_view& source);
};