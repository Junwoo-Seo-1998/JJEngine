/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/14/2023
End Header-------------------------------------------------------- */
#pragma once
#include <queue>
#include "Event/StringEvent.h"

class Scene;
class EventManager {
	using type = StringEvent;
	using Registry = entt::registry;
	std::queue<type> eventQueue{};
	//std::vector<type> events{};
	Registry* registry{};

public:
	//EventManager(Scene* sc);
	EventManager(Registry* reg);
	void notifyEvent(type e);
	void Update();
};