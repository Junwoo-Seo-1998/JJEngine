/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"
#include "ComponentManager.h"

Scene::Scene():components(std::make_shared<ComponentManager>()),ID(components->GetNewEntityID())
{
}

void Scene::Update(double dt)
{
	components->Update(dt);
}
