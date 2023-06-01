#include "EventManager.h"

void EventManager::AddEvent(std::shared_ptr<Event> Event)
{
	eventQueue.push(Event);
}

std::shared_ptr<Event> EventManager::PollEvent()
{
	if (eventQueue.empty())
		return nullptr;
	std::shared_ptr<Event> event = eventQueue.front();
	eventQueue.pop();
	return event;
}
