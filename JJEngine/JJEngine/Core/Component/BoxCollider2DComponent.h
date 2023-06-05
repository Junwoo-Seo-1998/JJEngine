#pragma once
#include <glm/vec2.hpp>

struct BoxCollider2DComponent
{
	glm::vec2 Offset = { 0.0f,0.0f };
	glm::vec2 Size = { 0.5f, 0.5f };

	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshold = 0.5f;

	// Storage for runtime
	void* RuntimeFixture = nullptr;

	BoxCollider2DComponent() = default;
	BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;
};
