#pragma once
#include "DebugDetection.h"
#ifdef JJ_ENABLE_ASSERTS
#define DEBUG_ONLY(x) x
#define RELEASE_ONLY(x)
#else
#define DEBUG_ONLY(x)
#define RELEASE_ONLY(x) x
#endif