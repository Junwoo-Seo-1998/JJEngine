#pragma once
#include <entt/entt.hpp>
//default component
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/NameComponent.h"
#include "Core/Entity/UUIDComponent.h"
#include "Core/Entity/RelationshipComponent.h"

class Scene;
class Entity
{
	friend class Scene;
public:
	Entity() = default;
	Entity(entt::entity entity_handle, Scene* scene);
	Entity(const Entity& other) = default;

	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	bool operator<(const Entity& other) const;

	//check it's valid entity
	operator bool() const;
	

	Entity GetParent() const;
	void SetParent(Entity parent);

	UUIDType GetUUID() const;
	UUIDType GetParentUUID() const;
	void SetParentUUID(UUIDType parentUUID);

	//to Set Children use this function
	std::vector<UUIDType>& GetChildrenUUID();
	const std::vector<UUIDType>& GetChildrenUUID() const;

	bool RemoveChild(Entity child);

	//default get set
	std::string& Name();
	const std::string& Name() const;
	TransformComponent& Transform();
	const TransformComponent& Transform() const;

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
	inline static std::string s_EmptyName = "UnnamedEntity";
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

