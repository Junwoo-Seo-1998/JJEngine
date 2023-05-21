/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"
#include "ComponentManager.h"

Scene::Scene():components(std::make_shared<ComponentManager>()),id(components->GetNewEntityID())
{
}

void Scene::Update(double dt)
{
	//components->Update(dt);
}

std::shared_ptr<ComponentManager>& Scene::GetCM()
{
	return components;
}
