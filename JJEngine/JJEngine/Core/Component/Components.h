#pragma once
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ScriptComponent.h"
#include "RigidBody2DComponent.h"
#include "BoxCollider2DComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "EnvironmentComponent.h"
#include "LightComponent.h"
template<typename ... Comp>
struct ComponentGroup
{
	
};

using AllComponents =
ComponentGroup
<
	CameraComponent,
	TransformComponent,
	SpriteRendererComponent,
	ScriptComponent,
	RigidBody2DComponent,
	BoxCollider2DComponent,
	LightComponent,
	MeshComponent,
	EnvironmentComponent
>;
