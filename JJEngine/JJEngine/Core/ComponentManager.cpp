/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/21/2022
End Header-------------------------------------------------------- */
#include "ComponentManager.h"
#include "entt/entt.hpp"

struct ComponentManager::Components{
	entt::registry registry{};
	Components() {}
	~Components() {}
};

ComponentManager::ComponentManager()
{
	components = std::make_shared<Components>();
}

ComponentManager::~ComponentManager()
{

}

template<typename T>
void ComponentManager::AddComponent(T& data)
{
	auto entity = components->registry.create();
	components->registry.emplace<T>(entity, data);
}

template<typename T>
void ComponentManager::RemoveComponents() {
	auto view = components->registry.view<T>();
	for (auto entity: view) {
		components->registry.destroy(entity);
	}
}

template<typename T>
std::vector<T&> ComponentManager::GetComponents() {
	std::vector<T&> data{};
	auto view = components->registry.view<T>();
	for (auto entity : view) {
		data.push_back(components->registry.get<T>(entity));
	}
	return data;
}