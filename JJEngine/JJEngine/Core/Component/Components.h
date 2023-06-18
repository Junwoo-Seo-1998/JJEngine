#pragma once
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ScriptComponent.h"
#include "RigidBody2DComponent.h"
#include "BoxCollider2DComponent.h"


template<typename ... Comp>
struct ComponentGroup
{
	
};

using AllComponents =
ComponentGroup
<
	TransformComponent,
	SpriteRendererComponent,
	ScriptComponent,
	RigidBody2DComponent,
	BoxCollider2DComponent
>;
