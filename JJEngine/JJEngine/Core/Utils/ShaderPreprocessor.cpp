#include "ShaderPreprocessor.h"

#include <regex>
#include <sstream>
#include <set>
#include <stack>

#include "File.h"
#include "Log.h"

#define PRAGMA "pragma"
#define INCLUDE "include"
#define DEFINE "define"
#define IFDEF "ifdef"
#define IFNDEF "ifndef"
#define ENDIF "endif"

#define VERT "vert"
#define FRAG "frag"
#define GEOM "geom"
#define TESS "tess"

std::vector<bool> completedShader(static_cast<int>(ShaderType::TypeCounts));

void Testing(std::string& sourceString);
ShaderType TranslateShaderType(const std::string& type);
const std::string TranslateShaderTypeToStr(const ShaderType& type);
std::vector<std::string_view> SplitStringWithCommand(const std::string_view& sourceString, std::string regex_command);
std::vector<std::string> SplitStringAndKeepDelims(std::string str)
{
	
	const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

	std::regex_iterator<std::string::iterator> rit(str.begin(), str.end(), re);
	std::regex_iterator<std::string::iterator> rend;
	std::vector<std::string> result;

	while (rit != rend)
	{
		result.emplace_back(rit->str());
		++rit;
	}
	return result;
}
std::unordered_map<ShaderType, std::string> ShaderPreprocesssor::Preprocess(const std::filesystem::path& file)
{
	std::string source = File::ReadFileToString(file.string());
	EngineLog::Trace("Shader Src Before Removing Comment : \n{}", source);
	source = CopyWithoutComments(source);
	EngineLog::Warn("Shader Src After Removing Comment : \n{}", source);

	auto delim = SplitStringAndKeepDelims(source);
	for (auto str:delim)
	{
		EngineLog::Warn("Delims : {}", str);
	}

	std::unordered_map<ShaderType, std::string> shaders{};
	std::string_view temp{source};
	//auto now=std::chrono::system_clock::now();
	CommandProcessing(shaders, ShaderType::None, temp);
	//auto end = std::chrono::system_clock::now();
	//auto duration = end - now;
	//EngineLog::Info("Time took:{}", std::chrono::duration_cast<std::chrono::milliseconds>(duration));

	for (auto & t: shaders) {
		EngineLog::Debug("Current shader: {}", TranslateShaderTypeToStr(t.first));
		EngineLog::Warn(t.second);
	}

	//Testing(source);

	// warning
	
	
	// clear settings
	completedShader = std::vector<bool>(static_cast<int>(ShaderType::TypeCounts));

	return shaders;
}


// From https://wandbox.org/permlink/iXC7DWaU8Tk8jrf3
enum class State : char { SlashOC, StarIC, SingleLineComment, MultiLineComment, NotAComment };
std::string ShaderPreprocesssor::CopyWithoutComments(const std::string& sourceString)
{
	State state = State::NotAComment;
	auto first = sourceString.begin();
	auto last = sourceString.end();


	std::stringstream sourceStream;
	auto out = std::ostream_iterator<char>(sourceStream);

	while (first != last)
	{
		switch (state)
		{
		case State::SlashOC:
			if (*first == '/') state = State::SingleLineComment;
			else if (*first == '*') state = State::MultiLineComment;
			else
			{
				state = State::NotAComment;
				*out++ = '/';
				*out++ = *first;
			}
			break;
		case State::StarIC:
			if (*first == '/') state = State::NotAComment;
			else state = State::MultiLineComment;
			break;
		case State::NotAComment:
			if (*first == '/') state = State::SlashOC;
			else *out++ = *first;
			break;
		case State::SingleLineComment:
			if (*first == '\n')
			{
				state = State::NotAComment;
				*out++ = '\n';
			}
			break;
		case State::MultiLineComment:
			if (*first == '*') state = State::StarIC;
			else if (*first == '\n') *out++ = '\n';
			break;
		}
		++first;
	}

	return sourceStream.str();
}

void ShaderPreprocesssor::CommandProcessing(std::unordered_map<ShaderType, std::string>& shaders, ShaderType current_shader_type, std::string_view& source)
{
	const static std::regex re(R"([#][^\w]*(\w+)([^#]*))", std::regex_constants::optimize);
	std::cmatch m;
	//auto now = std::chrono::system_clock::now();
	bool success = std::regex_search(source.data(), m, re);
	//auto end = std::chrono::system_clock::now();
	//auto duration = end - now;
	//EngineLog::Info("Time took:{}", std::chrono::duration_cast<std::chrono::milliseconds>(duration));
	if (success == false) {
		shaders[current_shader_type] += source.data();
		return;
	}
	shaders[current_shader_type] += m.prefix();
	std::string_view command{m[1].first, m[1].second};
	std::string_view content{m[2].first, m[2].second};


	if (command == PRAGMA) {
		std::vector<std::string_view> contentSplit{SplitStringWithCommand(content, R"((\w+)([^#]*))")};
		ShaderType commandOption{ TranslateShaderType(std::string{ contentSplit[1].begin(),contentSplit[1].end()}) };
		ENGINE_ASSERT(commandOption != ShaderType::None, "Not available shader type");
		ENGINE_ASSERT(commandOption != current_shader_type, "Continuously repeated shader type");
		ENGINE_ASSERT(completedShader[static_cast<int>(commandOption)] == false, "Repeated shader type");
		completedShader[static_cast<int>(current_shader_type)] = true;
		current_shader_type = commandOption;
		//shaders[current_shader_type] += std::string{contentSplit[2].begin(), contentSplit[2].end()};
		content = contentSplit[2];
	}
	else if (command == INCLUDE) {
		std::vector<std::string_view> contentSplit{SplitStringWithCommand(content, R"(["]([^"]*)["]([^#]*))")};
		std::string_view commandOption{contentSplit[1]};
		//file read
		std::string includePath{ commandOption.begin(), commandOption.end()};
		bool pathCheck{ File::CheckExists(includePath) };
		// relative path check
		std::string include_source = File::ReadFileToString(includePath);
		std::string_view temp{include_source};
		CommandProcessing(shaders, current_shader_type, temp);
		//shaders[current_shader_type] +=  std::string{contentSplit[2].begin(), contentSplit[2].end()};
		content = contentSplit[2];
	}
	else {
		shaders[current_shader_type] += "#";
		shaders[current_shader_type] += command;
	}

	shaders[current_shader_type] += content;

	//re-parsing m[4]
	std::string_view last{m.suffix().first, m.suffix().second};
	CommandProcessing(shaders, current_shader_type, last);
}


void Testing(std::string& sourceString)
{
	std::string str{sourceString};
	const static std::regex re(R"([#][^\w]*(\w+)([^#]*))", std::regex_constants::optimize);
	std::smatch m;
	bool res{std::regex_search(str, m, re)};
	ENGINE_ASSERT(res, "");


	EngineLog::Warn("Testing regex prefix : {}", m.prefix());
	EngineLog::Warn("Testing regex[0] : {}", m[0].str());
	EngineLog::Warn("Testing regex[1] : {}", m[1].str());
	EngineLog::Warn("Testing regex[2] : {}", m[2].str());
	EngineLog::Warn("Testing regex suffix : {}", m.suffix());
	//EngineLog::Warn("Testing regex[5] : {}", m[5].str());
	//EngineLog::Warn("Testing regex[6] : {}", m[7].str());

	//for (auto x : m)
	//	EngineLog::Warn("Testing regex : {}", x.str());

}

ShaderType TranslateShaderType(const std::string& type)
{
	if (type == VERT) {
		return ShaderType::VertexShader;
	}
	else if (type == FRAG) {
		return ShaderType::FragmentShader;
	}
	else if (type == GEOM) {
		return ShaderType::GeometryShader;
	}
	else if (type == TESS) {
		return ShaderType::TessellationShader;
	}
	return ShaderType::None;
}

const std::string TranslateShaderTypeToStr(const ShaderType& type)
{
	switch (type)
	{
	case ShaderType::VertexShader: return VERT;
	case ShaderType::FragmentShader: return FRAG;
	case ShaderType::GeometryShader: return GEOM;
	case ShaderType::TessellationShader: return TESS;
	default:
		return "None";
	}
}

std::vector<std::string_view> SplitStringWithCommand(const std::string_view& sourceString, std::string regex_command)
{
	const std::regex reg(regex_command);
	std::cmatch m{};
	bool res{ std::regex_search(sourceString.data(), m, reg) };
	ENGINE_ASSERT(res, "Can't find Command option");

	std::vector<std::string_view> result;
	for (auto& s:m) {
		result.emplace_back(s.first,s.second);
	}
	return result;
}

