#pragma once
struct RigidBody2DComponent
{
	enum class BodyType { Static = 0, Dynamic, Kinematic };
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;

	void* RuntimeBody = nullptr;

	RigidBody2DComponent() = default;
	RigidBody2DComponent(const RigidBody2DComponent& other) = default;
};
