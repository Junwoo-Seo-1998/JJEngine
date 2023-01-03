/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <vector>
#include "entt.h"

class ComponentManager {
	entt::registry registry{};
public:
	ComponentManager();
	~ComponentManager();

	entt::registry& GetRegistry();

	ID GetNewEntityID();
	void DeleteEntityID(ID id);

	/*template<typename T>
	void AddComponent(ID id, T& data);*/
	template<typename T>
	void AddComponent(ID id, T&& data);

	template<typename T>
	void RemoveComponent(ID id);
	template<typename T>
	T& GetComponent(ID id);

	template<typename T>
	void RemoveComponents();

	template<typename T>
	std::vector<T*> GetComponents();

	template<typename T>
	auto HasTEntities();
};

//template<typename T>
//void ComponentManager::AddComponent(ID id, T& data)
//{
//	registry.emplace<T>(id, data);
//}

template<typename T>
inline void ComponentManager::AddComponent(ID id, T&& data)
{
	registry.emplace<T>(id, data);
}

template<typename T>
inline void ComponentManager::RemoveComponent(ID id)
{
	registry.erase<T>(id);
}

template<typename T>
inline T& ComponentManager::GetComponent(ID id)
{
	return registry.get<T>(id);
}

template<typename T>
void ComponentManager::RemoveComponents() {
	auto view = registry.view<T>();
	for (auto entity : view) {
		registry.erase<T>(entity);
	}
}

template<typename T>
inline auto ComponentManager::HasTEntities()
{
	return registry.view<T>();
}

template<typename T>
std::vector<T*> ComponentManager::GetComponents() {
	std::vector<T*> data{};
	auto view = registry.view<T>();
	for (auto entity : view) {
		data.push_back(&registry.get<T>(entity));
	}
	return data;
}
