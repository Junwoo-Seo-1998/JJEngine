#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
public:
	//radian!! use glm::radians (degree)
	float Fov_y = glm::radians(45.f);
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 1000000.f;
	bool IsMainCamera = false;
	[[nodiscard]] glm::mat4 GetProjection() const;
};