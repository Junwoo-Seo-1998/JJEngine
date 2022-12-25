/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include "entt.h"

class ComponentManager;
class Entity{
protected:
	ID id{};
	std::shared_ptr<ComponentManager> components;
public:
	Entity(std::shared_ptr<ComponentManager> cm);
	virtual void Update(double ) {};
};
