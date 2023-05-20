#include "StringEvent.h"

#include "Core/Utils/UUIDGenerator.h"

StringEvent::StringEvent(const std::string& eventName)
	:eventID(UUIDGenerator::Generate(eventName))
{

}

StringEvent::StringEvent(const StringEvent& r_event)
	:eventID(r_event.eventID)
{
}
