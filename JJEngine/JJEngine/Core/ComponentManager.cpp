/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "ComponentManager.h"
#include "Entity.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

entt::entity ComponentManager::GetNewEntityID()
{
	return registry.create();
}