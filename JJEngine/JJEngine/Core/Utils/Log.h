#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class EngineLog
{
public:

	template <typename... Args>
	static void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->trace(fmt, args);
	}
	template<typename T>
	static void Trace(const T& msg)
	{
		s_EngineLogger->trace(msg);
	}

	template <typename... Args>
	static void Info(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->info(fmt, args);
	}
	template<typename T>
	static void Info(const T& msg)
	{
		s_EngineLogger->info(msg);
	}

	template <typename... Args>
	static void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->warn(fmt, args);
	}
	template<typename T>
	static void Warn(const T& msg)
	{
		s_EngineLogger->warn(msg);
	}

	template <typename... Args>
	static void Error(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->error(fmt, args);
	}
	template<typename T>
	static void Error(const T& msg)
	{
		s_EngineLogger->error(msg);
	}

	template <typename... Args>
	static void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->critical(fmt, args);
	}
	template<typename T>
	static void Critical(const T& msg)
	{
		s_EngineLogger->critical(msg);
	}

	template <typename... Args>
	static void Debug(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_EngineLogger->debug(fmt, args);
	}
	template<typename T>
	static void Debug(const T& msg)
	{
		s_EngineLogger->debug(msg);
	}
private:
	static std::shared_ptr<spdlog::logger> s_EngineLogger;
};


class Log
{
public:
	template <typename... Args>
	static void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->trace(fmt, args);
	}
	template<typename T>
	static void Trace(const T& msg)
	{
		s_ClientLogger->trace(msg);
	}

	template <typename... Args>
	static void Info(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->info(fmt, args);
	}
	template<typename T>
	static void Info(const T& msg)
	{
		s_ClientLogger->info(msg);
	}

	template <typename... Args>
	static void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->warn(fmt, args);
	}
	template<typename T>
	static void Warn(const T& msg)
	{
		s_ClientLogger->warn(msg);
	}

	template <typename... Args>
	static void Error(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->error(fmt, args);
	}
	template<typename T>
	static void Error(const T& msg)
	{
		s_ClientLogger->error(msg);
	}

	template <typename... Args>
	static void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->critical(fmt, args);
	}
	template<typename T>
	static void Critical(const T& msg)
	{
		s_ClientLogger->critical(msg);
	}

	template <typename... Args>
	static void Debug(spdlog::format_string_t<Args...> fmt, Args &&... args)
	{
		s_ClientLogger->debug(fmt, args);
	}
	template<typename T>
	static void Debug(const T& msg)
	{
		s_ClientLogger->debug(msg);
	}
private:
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
