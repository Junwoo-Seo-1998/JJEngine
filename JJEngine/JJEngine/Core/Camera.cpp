/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "Camera.h"
#include "Input/Input.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


void FreeCamera::ComputeViewProjMats()
{
	topPlane = 0.6f * nearPlane;
	bottomPlane = -topPlane;
	rightPlane = topPlane * aspect;
	leftPlane = -rightPlane;
	/*  Update view transform matrix */
	viewMat = glm::lookAt(cam, cam + camFront, camUp);

	/*  Update projection matrix */
	projMat = glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

	vpMat = projMat * viewMat;
}

glm::mat4 FreeCamera::ComputeCenterMapping(int i)
{

	/*  Update view transform matrix */
	//const Vec3 centerCam = { 0.f, 0.f, 0.f };
	glm::vec3 CenterCamFront;
	glm::vec3 CenterCamUp = camUp;
	switch (i)
	{
	case 0:
		CenterCamFront = { 0.f, 0.f, 1.f };
		break;
	case 1:
		CenterCamFront = { -1.f, 0.f, 0.f };
		break;
	case 2:
		CenterCamFront = { 1.f, 0.f, 0.f };
		break;
	case 3:
		CenterCamFront = { 0.f, 0.f, -1.f };
		break;
	case 4:
		CenterCamFront = { 0.f, 1.f, 0.f };
		CenterCamUp = { 0.f, 0.f, 1.f };
		break;
	case 5:
		CenterCamFront = { 0.f, -1.f, 0.f };
		CenterCamUp = { 0.f, 0.f, -1.f };
		break;
	}
	glm::vec3 CenterCam(0.f);
	viewMat = glm::lookAt(CenterCam, CenterCam + CenterCamFront, CenterCamUp);

	/*  Update projection matrix */
	projMat = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 100.f);
	glm::mat4 view = viewMat;
	return projMat * view;
}

void FreeCamera::Reset()
{
	cam = { 0,0,20 };
	camFront = { 0,0,-0.1 };
	camUp = { 0, 1, 0 };
	yaw = 270;
	pitch = 0;
}

void FreeCamera::Update()
{
	if (Input::IsReleased(KeyCode::Space)) {
		camMove = !camMove;
	}
	if (camMove) {
		Input::SetMouseCursorEnable(false);
	}
	else {
		Input::SetMouseCursorEnable(true);
	}

	if (Input::IsPressed(KeyCode::W)) {
		cam += camspeed * camFront;
	}
	if (Input::IsPressed(KeyCode::S)) {
		cam -= camspeed * camFront;
	}
	if (Input::IsPressed(KeyCode::A)) {
		cam -= glm::normalize(glm::cross(camFront, camUp)) * camspeed;
	}
	if (Input::IsPressed(KeyCode::D)) {
		cam += glm::normalize(glm::cross(camFront, camUp)) * camspeed;
	}
	if (Input::IsPressed(KeyCode::Q)) {
		cam.y += camspeed;
	}
	if (Input::IsPressed(KeyCode::E)) {
		cam.y -= camspeed;
	}

	mouseCam(get<0>(Input::GetMousePosition()), get<1>(Input::GetMousePosition()));
}

void FreeCamera::SetCamSpeed(float speed) {
	camspeed = speed;
}

void FreeCamera::SetPosition(glm::vec3 pos) {
	cam = pos;
}


void FreeCamera::mouseCam(double xpos, double ypos)
{
	if (camMove)
	{
		double xoffset = get<0>(Input::GetMouseOffset());
		double yoffset = get<1>(Input::GetMouseOffset());

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += static_cast<float>(xoffset);
		pitch += static_cast<float>(yoffset);

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camFront = glm::normalize(front);

	}
}

