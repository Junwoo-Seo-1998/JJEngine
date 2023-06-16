#pragma once
#include "Core/Type.h"
class UUIDHelper
{
public:
	static UUIDType MakeUUIDWithUINT64(uint64_t low, uint64_t high);
};
