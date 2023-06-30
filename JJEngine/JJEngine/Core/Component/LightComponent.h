#pragma once
#include "Core/Graphics/Light.h"
#include "glm/glm.hpp"

struct LightComponent
{
public:
	glm::mat4 GetProjection() const;
public:
	Light light{};
};