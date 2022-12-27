/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/24/2022
End Header-------------------------------------------------------- */
#include "Entity.h"
#include "ComponentManager.h"

Entity::Entity(std::shared_ptr<ComponentManager> cm):components(cm)
{
}

Entity::Entity(std::shared_ptr<ComponentManager> cm, ID newID): components(cm), id(newID)
{
}

void Entity::SetID(ID newID) {
	id = newID;
}
ID Entity::GetID() { return id; }