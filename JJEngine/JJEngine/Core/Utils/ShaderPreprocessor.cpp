#include "ShaderPreprocessor.h"

#include <regex>
#include <sstream>

#include "File.h"
#include "Log.h"


// R"(([^#]*)([#])(\w+) +("[^"]*"|\w+)([^#]*)([^]*))"
#define SHARP_COMMAND R"(([^#]*)([#])(\w+)([^#]*)([^]*))"
#define PRAGMA "pragma"
#define INCLUDE "include"

#define VERT "vert"
#define FRAG "frag"
#define GEOM "geom"
#define TESS "tess"

std::vector<bool> completedShader(static_cast<int>(ShaderType::TypeCounts));

ShaderType TranslateShaderType(const std::string& type);
const std::string TranslateShaderTypeToStr(const ShaderType& type);
std::vector<std::string> SplitStringWithCommand(const std::string& sourceString, std::string regex_command);
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
void ShaderPreprocesssor::Preprocess(const std::filesystem::path& file)
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

	//
	std::unordered_map<ShaderType, std::string> shaders{};
	completedShader = std::vector<bool>(static_cast<int>(ShaderType::TypeCounts));
	CommandProcessing(shaders, ShaderType::None, source);

	for (auto & t: shaders) {
		EngineLog::Debug("Current shader: {}", TranslateShaderTypeToStr(t.first));
		EngineLog::Warn(t.second);
	}

	//Testing(source);
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


void ShaderPreprocesssor::Testing(std::string& sourceString)
{
	std::string str{sourceString};
	const static std::regex re(R"(([^#]*)([#])(\w+)([^#]*)([^]*))", std::regex_constants::optimize);
	std::smatch m;
	ENGINE_ASSERT(std::regex_search(str,m,re),"");

	EngineLog::Warn("Testing regex[0] : {}", m[0].str());
	EngineLog::Warn("Testing regex[1] : {}", m[1].str());
	EngineLog::Warn("Testing regex[2] : {}", m[2].str());
	EngineLog::Warn("Testing regex[3] : {}", m[3].str());
	EngineLog::Warn("Testing regex[4] : {}", m[4].str());
	EngineLog::Warn("Testing regex[5] : {}", m[5].str());
	EngineLog::Warn("Testing regex[6] : {}", m[7].str());
	
	//for (auto x : m)
	//	EngineLog::Warn("Testing regex : {}", x.str());
	
}

void ShaderPreprocesssor::CommandProcessing(std::unordered_map<ShaderType, std::string>& shaders, ShaderType current_shader_type, std::string& source)
{
	const static std::regex re(R"(([^#]*)[#](\w+)([^#]*)([^]*))", std::regex_constants::optimize);
	std::smatch m;
	bool success = std::regex_search(source, m, re);
	if (success == false) {
		shaders[current_shader_type] += source;
		return;
	}

	shaders[current_shader_type] += m[1].str();
	std::string command{m[2].str()};
	std::string content{m[3].str()};

	if (command == PRAGMA) {
		std::vector<std::string> contentSplit{SplitStringWithCommand(content, R"((\w+)([^]*))")};
		ShaderType commandOption{ TranslateShaderType(contentSplit[1])};
		bool isAvailableType{ commandOption != ShaderType::None };
		ENGINE_ASSERT(commandOption != ShaderType::None, "Not available shader type");
		ENGINE_ASSERT(commandOption != current_shader_type, "Continuously repeated shader type");
		ENGINE_ASSERT(completedShader[static_cast<int>(commandOption)] == false, "Repeated shader type");
		completedShader[static_cast<int>(current_shader_type)] = true;
		current_shader_type = commandOption;
		content = contentSplit[2];
	}
	else if (command == INCLUDE) {
		std::vector<std::string> contentSplit{SplitStringWithCommand(content, R"(["]([^]*)["]([^]*))")};
		std::string commandOption{contentSplit[1]};
		//file check
		//file read
		//file put
		std::string include_source = File::ReadFileToString(commandOption);
		CommandProcessing(shaders, current_shader_type, include_source);
		content = contentSplit[2];
	}
	else {
		ENGINE_ASSERT(false, "Not available command");
	}
	shaders[current_shader_type] += content;

	//re parsing m[4]
	std::string last{m[4].str()};
	CommandProcessing(shaders, current_shader_type, last);
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

std::vector<std::string> SplitStringWithCommand(const std::string& sourceString, std::string regex_command)
{
	const std::regex reg(regex_command, std::regex_constants::optimize);
	std::smatch m;
	ENGINE_ASSERT(std::regex_search(sourceString, m, reg), "Can't find Command option");

	std::vector<std::string> result;
	for (auto& s:m) {
		result.emplace_back(s.str());
	}
	return result;
}

