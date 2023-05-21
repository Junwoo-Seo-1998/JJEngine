#pragma once
#include "uuid.h"
class StringEvent
{
	using StringEventID = uuids::uuid;
public:
	StringEvent() = delete;
	StringEvent(const std::string& eventName);
	StringEvent(const StringEvent& r_event);
	bool operator==(const StringEvent& r_event) const;
private:
	StringEventID eventID;
};