#pragma once
#include "Core/Graphics/Light.h"
#include "glm/glm.hpp"

struct LightComponent
{
public:
	Light light;



	//reserved

	//radian!! use glm::radians (degree)
	float Fov_y = glm::radians(90.1f);
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 100.f;

	LightType type = LightType::PointLight;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	float topPlane = 0.6f * nearPlane;
	float bottomPlane = -topPlane;
	float aspect = 1.0f;
	float rightPlane = topPlane * aspect;
	float leftPlane = -rightPlane;


	[[nodiscard]] glm::mat4 GetProjection() const;
};