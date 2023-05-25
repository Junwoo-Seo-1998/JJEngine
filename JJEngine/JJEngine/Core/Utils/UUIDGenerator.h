#pragma once
#include "Core/Type.h"

class UUIDGenerator
{
public:
	static UUIDType Generate();
	static UUIDType Generate(const std::string& str);
};
