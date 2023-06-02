#pragma once
#include "glm/glm.hpp"

struct CameraComponent
{
public:
	float Fov_y = 45.f; //degree
	float Aspect_ratio = 1.0f;
	float Near = 1.0f;
	float Far = 1000000.f;
	bool IsMainCamera = false;
	[[nodiscard]] glm::mat4 GetMatrix() const;
};