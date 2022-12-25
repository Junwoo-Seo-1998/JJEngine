/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/24/2022
End Header-------------------------------------------------------- */
#include "Entity.h"
#include "ComponentManager.h"

Entity::Entity(std::shared_ptr<ComponentManager> cm):components(cm)
{
}
