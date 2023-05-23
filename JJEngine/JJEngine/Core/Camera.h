/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include "glm/glm.hpp"
class GLFWwindow;
class FreeCamera 
{
public:
	float nearPlane = 0;
	float farPlane = 0;
	float topPlane = 0;
	float bottomPlane = 0;
	float aspect = 0;
	float rightPlane = 0;
	float leftPlane = 0;

	float yaw = 270;
	float pitch = 0;
	float roll = 0;
	float camspeed = 0.15f;
	glm::vec3 cam{};
	glm::vec3 camFront{};
	glm::vec3 camUp{};
	glm::mat4 viewMat{}, projMat{}, magMat{}, vpMat{};
	GLFWwindow* window_ptr;
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
	bool camMove = false;

public:
	FreeCamera(GLFWwindow* window_ptr, int windowWidth, int windowHeight) : window_ptr(window_ptr)
	{
		Reset();
		camMove = false;
		nearPlane = 0.1f;
		farPlane = 100.0f;
		topPlane = 0.6f * nearPlane;
		bottomPlane = -topPlane;
		aspect = 1.0f * windowWidth / windowHeight;
		rightPlane = topPlane * aspect;
		leftPlane = -rightPlane;
	}
	FreeCamera()
	{
		window_ptr = nullptr;
	};
	void ComputeViewProjMats();
	glm::mat4 ComputeCenterMapping(int i);

	void Reset();
	void Update();
	void SetCamSpeed(float speed);
	void SetPosition(glm::vec3 pos);
	void mouseCam(double, double);
};

