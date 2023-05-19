#pragma once
#include "uuid.h"

using UUID = uuids::uuid;

class UUIDGenerator
{
public:
	static UUID Generate();
	static UUID Generate(const std::string& str);
};