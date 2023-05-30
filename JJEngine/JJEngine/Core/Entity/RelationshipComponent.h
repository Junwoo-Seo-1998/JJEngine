#pragma once
#include "Core/Type.h"
struct RelationshipComponent
{
	RelationshipComponent() = default;
	RelationshipComponent(const RelationshipComponent& other) = default;
	RelationshipComponent(UUIDType parent)
		: Parent(parent) {}

	UUIDType Parent;
	std::vector<UUIDType> Children;
};
