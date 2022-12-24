#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name)
{
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
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
