#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
public:
	bool main_cam = false;
	float cam_speed = 1.f;
	float fov_y = 45.f; //degree
	float aspect_ratio = 1.0f;
	float near = 1.0f;
	float far = 1000000.f;
	[[nodiscard]] glm::mat4 GetMatrix() const;
};