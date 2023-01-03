/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "ComponentManager.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

entt::registry& ComponentManager::GetRegistry()
{
	return registry;
}

entt::entity ComponentManager::GetNewEntityID()
{
	return registry.create();
}

void ComponentManager::DeleteEntityID(ID id)
{
	registry.destroy(id);
}
