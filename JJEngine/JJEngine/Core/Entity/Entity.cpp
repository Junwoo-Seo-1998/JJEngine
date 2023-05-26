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

void Entity::SetParent(Entity parent)
{
	Entity currentParent = GetParent();
	if (currentParent == parent)
		return;

	// if there is parent already remove child from it
	if (currentParent)
		currentParent.RemoveChild(*this);

	SetParentUUID(parent.GetUUID());

	// if it's existing entity add this to children
	if (parent)
	{
		std::vector<UUIDType>& children = parent.GetChildrenUUID();
		UUIDType uuid = GetUUID();
		//check this is already child of the parent
		if (std::find(children.begin(), children.end(), uuid) == children.end())
			children.emplace_back(GetUUID());
	}
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

bool Entity::RemoveChild(Entity child)
{
	//(jun) : when remove child from parent should make the child to be null(?)
	UUIDType toDelete = child.GetUUID();

	std::vector<UUIDType>& children = GetChildrenUUID();
	auto found = std::find(children.begin(), children.end(), toDelete);
	if (found != children.end())
	{
		children.erase(found);
		return true;
	}

	return false;
}

std::string& Entity::Name()
{
	return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName;
}

const std::string& Entity::Name() const
{
	return HasComponent<NameComponent>() ? GetComponent<NameComponent>().Name : s_EmptyName;
}

TransformComponent& Entity::Transform()
{
	return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle);
}

const TransformComponent& Entity::Transform() const
{
	return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle);
}
