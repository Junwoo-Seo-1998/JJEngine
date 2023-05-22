/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/14/2023
End Header-------------------------------------------------------- */
#include "entt/entt.hpp"
#include "EventManager.h"
#include "EventListener.h"
#include "Utils/Log.h"

void EventManager::notifyEvent(type e)
{
	eventQueue.push(e);
	Log::Info("event notified");
}

EventManager::EventManager(Registry* reg ) : registry(reg)
{
}


void EventManager::Update()
{
	auto listeners = registry->view<EventListener>();
	while (eventQueue.empty() == false) {
		type& temp = eventQueue.front();
		for (auto& e:listeners) {
			listeners.get<EventListener>(e).PushEvent(temp);
		}
		eventQueue.pop();
	}
}