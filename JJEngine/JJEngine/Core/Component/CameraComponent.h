#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
public:
	float fov_y = 45.f; //degree
	float aspect_ratio = 1.0f;
	float near = 1.0f;
	float far = 1000000.f;

	[[nodiscard]] glm::mat4 GetPerspective() const;
};