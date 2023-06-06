#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
public:
	//radian!! use glm::radians (degree)
	float cam_speed = 0.1f;
	float Fov_y = glm::radians(45.f);
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 1000000.f;
	bool main_cam = true;
	bool IsMainCamera = false;
	[[nodiscard]] glm::mat4 GetProjection() const;
};