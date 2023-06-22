#include "ShaderPreprocessor.h"

#include <regex>
#include <sstream>

#include "File.h"
#include "Log.h"

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

