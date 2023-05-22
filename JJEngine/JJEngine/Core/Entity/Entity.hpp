#pragma once
#include <entt/entt.hpp>
class Scene;
class Entity
{
public:
	Entity() = default;
	Entity(entt::entity entity_handle, Scene* scene) :m_EntityHandle(entity_handle), m_Scene(scene)
	{}
	Entity(const Entity& other) = default;

	template<typename Comp_type, typename... Args>
	Comp_type& AddComponent(Args&&... args);

	template<typename Comp_type>
	bool HasComponent() const;

	template <typename Comp_type>
	Comp_type& GetComponent();

	template <typename Comp_type>
	void RemoveComponent();
private:
	entt::entity m_EntityHandle = entt::null;
	//for easy adding components
	Scene* m_Scene = nullptr;
};

template <typename Comp_type, typename ... Args>
Comp_type& Entity::AddComponent(Args&&... args)
{
	Comp_type& comp = m_Scene->m_Registry.emplace<Comp_type>(m_EntityHandle, std::forward<Args>(args)...);
	return comp;
}

template <typename Comp_type>
bool Entity::HasComponent() const
{
	return m_Scene->m_Registry.any_of<Comp_type>(m_EntityHandle);
}

template <typename Comp_type>
Comp_type& Entity::GetComponent()
{
	return m_Scene->m_Registry.get<Comp_type>(m_EntityHandle);
}

template <typename Comp_type>
void Entity::RemoveComponent()
{
	m_Scene->m_Registry.remove<Comp_type>(m_EntityHandle);
}

