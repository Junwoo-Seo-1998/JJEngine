#pragma once
#include "PlatformDetection.h"

#ifdef JJ_DEBUG
#if defined(JJ_PLATFORM_WINDOWS)
#define JJ_DEBUGBREAK() __debugbreak()
#elif defined(JJ_PLATFORM_LINUX)
#include <signal.h>
#define JJ_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define JJ_ENABLE_ASSERTS
#else
#define JJ_DEBUGBREAK()
#endif