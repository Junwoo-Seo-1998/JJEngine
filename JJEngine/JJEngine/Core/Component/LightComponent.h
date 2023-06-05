#pragma once
#include "glm/glm.hpp"
enum class LightType
{
	Point,
};

struct LightComponent
{
public:
	float fov_y = 45.f; //degree
	float aspect_ratio = 1.0f;
	float near = 1.0f;
	float far = 1000000.f;
	LightType type = LightType::Point;

	[[nodiscard]] glm::mat4 GetMatrix() const;
};