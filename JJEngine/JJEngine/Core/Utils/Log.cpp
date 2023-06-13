#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name)
{
	std::vector<spdlog::sink_ptr> logSinks;
	auto dup_filter = std::make_shared<spdlog::sinks::dup_filter_sink_st>(std::chrono::seconds(5));
	dup_filter->add_sink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(dup_filter);
	//logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("JJEngine.log", true));

	logSinks[0]->set_pattern("%^[%T] %n: %v%$");
	//logSinks[1]->set_pattern("[%T] [%l] %n: %v");

	std::shared_ptr<spdlog::logger> Logger = std::make_shared<spdlog::logger>(name, begin(logSinks), end(logSinks));
	spdlog::register_logger(Logger);
	Logger->set_level(spdlog::level::trace);
	Logger->flush_on(spdlog::level::trace);
	return Logger;
}

std::shared_ptr<spdlog::logger> EngineLog::s_EngineLogger = CreateLogger("JJEngine");
std::shared_ptr<spdlog::logger> Log::s_ClientLogger = CreateLogger("Game");

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
	os << "\nVec 2:\n";
	os << std::format("[ {:>5.2f}, {:>5.2f} ]", vec.x, vec.y);
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	os << "\nVec 3:\n";
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f} ]", vec.x, vec.y, vec.z);
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
	os << "\nVec 4:\n";
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f}, {:>5.2f} ]", vec.x, vec.y, vec.z, vec.w);
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat3& mat)
{
	os << "\nMatrix 3 x 3:\n";
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f} ]\n", mat[0].x, mat[1].x, mat[2].x);
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f} ]\n", mat[0].y, mat[1].y, mat[2].y);
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f} ]", mat[0].z, mat[1].z, mat[2].z);
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	os << "\nMatrix 4 x 4:\n";
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f}, {:>5.2f} ]\n", mat[0].x, mat[1].x, mat[2].x, mat[3].x);
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f}, {:>5.2f} ]\n", mat[0].y, mat[1].y, mat[2].y, mat[3].y);
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f}, {:>5.2f} ]\n", mat[0].z, mat[1].z, mat[2].z, mat[3].z);
	os << std::format("[ {:>5.2f}, {:>5.2f}, {:>5.2f}, {:>5.2f} ]", mat[0].w, mat[1].w, mat[2].w, mat[3].w);
	return os;
}
