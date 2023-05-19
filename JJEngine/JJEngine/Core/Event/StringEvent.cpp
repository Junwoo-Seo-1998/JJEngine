#include "StringEvent.h"

#include "Core/Utils/UUIDGenerator.h"

StringEvent::StringEvent(const std::string& eventName)
	:eventID(UUIDGenerator::Generate(eventName))
{

}
