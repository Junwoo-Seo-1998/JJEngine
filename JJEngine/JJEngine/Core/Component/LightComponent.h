#pragma once
#include "glm/glm.hpp"
enum class LightType
{
	Point,
};

struct LightComponent
{
public:
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	float topPlane = 0.6f * nearPlane;
	float bottomPlane = -topPlane;
	float aspect = 1.0f;
	float rightPlane = topPlane * aspect;
	float leftPlane = -rightPlane;
	//radian!! use glm::radians (degree)
	float Fov_y = glm::radians(90.1f);
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 100.f;
	LightType type = LightType::Point;
	[[nodiscard]] glm::mat4 GetProjection() const;
};