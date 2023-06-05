#pragma once
#include <memory>
#include <queue>
#include "Event.h"

class EventManager
{
public:
	void AddEvent(std::shared_ptr<Event> Event);

	std::shared_ptr<Event> PollEvent();
private:
	std::queue<std::shared_ptr<Event>> eventQueue;
};

