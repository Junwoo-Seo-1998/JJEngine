#pragma once
#include <entt/entt.hpp>
#include "Core/Component/NameComponent.h"

class Scene;
class Entity
{
	friend class Scene;
public:
	Entity() = default;
	Entity(entt::entity entity_handle, Scene* scene) :m_EntityHandle(entity_handle), m_Scene(scene)
	{}
	Entity(const Entity& other) = default;

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	std::string& Name() { return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName; }
	const std::string& Name() const { return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName; }

	template<typename Comp_type, typename... Args>
	Comp_type& AddComponent(Args&&... args);

	template<typename Comp_type>
	bool HasComponent() const;

	template <typename Comp_type>
	Comp_type& GetComponent();

	template<typename Comp_type>
	const Comp_type& GetComponent() const;

	template <typename Comp_type>
	void RemoveComponent();
private:
	entt::entity m_EntityHandle = entt::null;
	Scene* m_Scene = nullptr;
	inline static std::string s_EmptyName = "UnnamedEnitiy";
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

template<typename Comp_type>
const Comp_type& Entity::GetComponent() const
{
	return m_Scene->m_Registry.get<Comp_type>(m_EntityHandle);
}

template <typename Comp_type>
void Entity::RemoveComponent()
{
	m_Scene->m_Registry.remove<Comp_type>(m_EntityHandle);
}

