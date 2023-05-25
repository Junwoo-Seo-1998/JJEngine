#include "Core/Scene.h"

//--include Entity at the end due to the template
#include "Entity.hpp"

Entity::Entity(entt::entity entity_handle, Scene* scene)
	:m_EntityHandle(entity_handle), m_Scene(scene) {}

bool Entity::operator==(const Entity& other) const
{
	return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
}

bool Entity::operator!=(const Entity& other) const
{
	return !(*this == other);
}

bool Entity::operator<(const Entity& other) const
{
	return m_EntityHandle < other.m_EntityHandle;
}

Entity::operator bool() const
{
	return (m_EntityHandle != entt::null) && (m_Scene != nullptr) && m_Scene->m_Registry.valid(m_EntityHandle);
}

Entity Entity::GetParent() const
{
	return m_Scene->TryGetEntity(GetParentUUID());
}

UUIDType Entity::GetUUID() const
{
	return GetComponent<UUIDComponent>().UUID;
}

UUIDType Entity::GetParentUUID() const
{
	return GetComponent<RelationshipComponent>().Parent;
}

void Entity::SetParentUUID(UUIDType parentUUID)
{
	GetComponent<RelationshipComponent>().Parent = parentUUID;
}

std::vector<UUIDType>& Entity::GetChildrenUUID()
{
	return GetComponent<RelationshipComponent>().Children;
}

const std::vector<UUIDType>& Entity::GetChildrenUUID() const
{
	return GetComponent<RelationshipComponent>().Children;
}

std::string& Entity::Name()
{
	return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName;
}

const std::string& Entity::Name() const
{
	return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName;
}
