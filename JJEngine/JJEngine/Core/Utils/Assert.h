#pragma once
#include "DebugDetection.h"
#include "Log.h"
#include <filesystem>
#ifdef JJ_ENABLE_ASSERTS

#define __EXPAND_MACRO(x) x
#define __STRINGIFY_MACRO(x) #x

#define __INTERNAL_ASSERT_WITH_MSG(check, msg, ...) { if(!(check)) { Log::Critical("Assertion '{0}' failed at {1} line {2}\n*Detail Error Message:\n{3}", __STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).string(), __LINE__, msg); JJ_DEBUGBREAK(); } }
#define __INTERNAL_ASSERT_NO_MSG(check) { if(!(check)) { Log::Critical("Assertion '{0}' failed at {1} line {2}", __STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).string(), __LINE__); JJ_DEBUGBREAK(); } }

#define __INTERNAL_ENGINE_ASSERT_WITH_MSG(check, msg, ...) { if(!(check)) { EngineLog::Critical("Assertion '{0}' failed at {1} line {2}\n*Detail Error Message:\n{3}", __STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).string(), __LINE__, msg); JJ_DEBUGBREAK(); } }
#define __INTERNAL_ENGINE_ASSERT_NO_MSG(check) { if(!(check)) { EngineLog::Critical("Assertion '{0}' failed at {1} line {2}", __STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).string(), __LINE__); JJ_DEBUGBREAK(); } }

#define __GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define __GET_MACRO(...) __EXPAND_MACRO( __GET_MACRO_NAME(__VA_ARGS__, __INTERNAL_ASSERT_WITH_MSG, __INTERNAL_ASSERT_NO_MSG) )
#define __GET_ENGINE_MACRO(...) __EXPAND_MACRO( __GET_MACRO_NAME(__VA_ARGS__, __INTERNAL_ENGINE_ASSERT_WITH_MSG, __INTERNAL_ENGINE_ASSERT_NO_MSG) )

#define ASSERT(...)  __EXPAND_MACRO(__GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
#define ENGINE_ASSERT(...) __EXPAND_MACRO(__GET_ENGINE_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
#define ASSERT(...)
#define ENGINE_ASSERT(...)
#endif