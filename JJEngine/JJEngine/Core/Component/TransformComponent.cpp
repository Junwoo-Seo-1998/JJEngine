#include "TransformComponent.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

glm::mat4 TransformComponent::GetTransform() const
{
	glm::mat4 current = glm::translate(glm::mat4(1.0f), Position)
		* glm::toMat4(glm::quat(Rotation))
		* glm::scale(glm::mat4(1.0f), Scale);
	return current;
}

glm::vec3 TransformComponent::GetRight() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 1.f,0.f,0.f ,0.f };
}

glm::vec3 TransformComponent::GetUp() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 0.f,1.f,0.f ,0.f };
}

glm::vec3 TransformComponent::GetForward() const
{
	return glm::toMat4(glm::quat(Rotation)) * glm::vec4{ 0.f,0.f,-1.f ,0.f };
}

void TransformComponent::LookAtDir(const glm::vec3& dir)
{
	// Is the normal up (nearly) parallel to direction?
	glm::vec3 up = glm::vec3{ 0.f,1.f,0.f };
	if (glm::abs(glm::dot(dir, up)) > .9999f) {
		// Use alternative up
		glm::vec3 result = glm::eulerAngles(glm::quatLookAt(glm::normalize(dir), { 0.f,1.01f,0.f }));
		Rotation = result;

	}

	glm::vec3 result = glm::eulerAngles(glm::quatLookAt(glm::normalize(dir), up));
	Rotation = result;
}

