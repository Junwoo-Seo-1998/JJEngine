#pragma once
#include <glm/glm.hpp>

struct TransformComponent
{
public:
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation)
		: Position(translation) {}

	glm::mat4 GetTransform() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetForward() const;
	void LookAtDir(const glm::vec3& dir);

	glm::vec3 Position{ 0.f };
	//radian!! use glm::radians (degree)
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale{ 1.f };
};