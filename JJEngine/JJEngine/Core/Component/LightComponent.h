#pragma once
#include "Core/Graphics/Light.h"
#include "glm/glm.hpp"

struct LightComponent
{
public:
	glm::mat4 GetProjection() const;
public:
	Light light{};
	float Fov_y = glm::radians(90.f);
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 100.f;
};