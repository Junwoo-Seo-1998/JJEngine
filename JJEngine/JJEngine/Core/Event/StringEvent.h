#pragma once
#include "uuid.h"
using StringEventID = uuids::uuid;
using UUIDNamespace = uuids::uuid;
class StringEvent
{
public:

	StringEventID eventID;
	StringEvent() = delete;
	StringEvent(const std::string& eventName);
	StringEvent(const StringEvent& r_event);
};