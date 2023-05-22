/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include "entt.h"

class ComponentManager;
class Scene
{
public:
	Scene();
	Scene(const std::string& scene_name);
	virtual ~Scene();

	virtual void Awake();
	virtual void OnEnable();
	virtual void Start();
	virtual void Update();
	virtual void PostUpdate();
	virtual void OnDisable();
	virtual void OnDestroy();

protected:
	std::string m_scene_name;
};