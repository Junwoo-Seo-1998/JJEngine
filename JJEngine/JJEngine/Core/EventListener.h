/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/14/2023
End Header-------------------------------------------------------- */
#pragma once
#include <queue>
#include "Event/StringEvent.h"
class EventListener {
	using type = StringEvent;
	std::queue<type> eventQueue{};
public:
	void PushEvent(type ev) { eventQueue.push(ev); }
	type GetNextEvent() { 
		if (eventQueue.empty() == true) return type{"None"};
		type& temp = eventQueue.front();
		eventQueue.pop();
		return temp; 
	}
};