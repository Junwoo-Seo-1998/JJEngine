/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "Camera.h"

Camera::Camera(float Fov, float AR, float Distance, float Near, float Far): 
	fov(Fov),ar(AR),distance(Distance),near(Near),far(Far)
{
}

void Camera::Update(double dt) {
	WHUpdate();

	matrixUpdateFunc();
}

void Camera::resetAngle()
{
	upDirection = { 0,1,0 };
	rightDirection = { 1,0,0 };
	backDirection = { 0,0,1 };
}

glm::vec3 Camera::getAngle()
{
	return CameraAngle;
}

void Camera::setRotate(glm::vec3 angle)
{
	CameraAngle = angle;
	glm::mat3 XRotMat{ rotationMat(glm::vec3{1,0,0},angle.x) };
	glm::mat3 YRotMat{ rotationMat(glm::vec3{0,-1,0},angle.y) };
	glm::mat3 ZRotMat{ rotationMat(glm::vec3{0,0,1},angle.z) };

	backDirection = glm::normalize((XRotMat * YRotMat * ZRotMat) * glm::vec3{ 0,0,1 });
	rightDirection = glm::normalize(glm::cross(glm::vec3{ 0,1,0 }, backDirection));
	upDirection = glm::normalize(glm::cross(backDirection, rightDirection));
	matrixUpdate = true;
}

void Camera::updateRotate(glm::vec3 angle)
{
	CameraAngle = CameraAngle + angle;
	glm::mat3 XRotMat{ rotationMat(glm::vec3{1,0,0},angle.x) };
	glm::mat3 YRotMat{ rotationMat(glm::vec3{0,-1,0},angle.y) };
	glm::mat3 ZRotMat{ rotationMat(glm::vec3{0,0,1},angle.z) };

	backDirection = glm::normalize((XRotMat)*backDirection);
	upDirection = glm::normalize(glm::cross(backDirection, rightDirection));

	backDirection = glm::normalize((YRotMat)*backDirection);
	rightDirection = glm::normalize(glm::cross(upDirection, backDirection));

	rightDirection = glm::normalize((ZRotMat)*rightDirection);
	upDirection = glm::normalize(glm::cross(backDirection, rightDirection));


	matrixUpdate = true;
}

void Camera::setLookAt(glm::vec3 /*lookAt*/)
{
}

void Camera::setLookAt(glm::vec3 lookAtVec, glm::vec3 rightVec, glm::vec3 upVec)
{
	backDirection = glm::normalize(-lookAtVec);
	rightDirection = glm::normalize(rightVec);
	upDirection = glm::normalize(upVec);
}

void Camera::WHUpdate() {

	height = tan(fov / 2.f) * 2.f * distance;
	width = ar * height;
}

void Camera::matrixUpdateFunc()
{
	if (matrixUpdate == true) {
		const float nearMinusFar = near - far;
		cameraToWorld = glm::mat4{ glm::vec4{rightDirection,0.f},glm::vec4{upDirection,0.f},glm::vec4{backDirection,0.f},glm::vec4{position,1.f} };
		worldToCamera = glm::inverse(cameraToWorld);
		cameraToNDC = glm::mat4{
			glm::vec4{2.f * distance / width,0.f,0.f,0.f},
			glm::vec4{0.f,2.f * distance / height,0.f,0.f},
			glm::vec4{0.f,0.f,(near + far) / (nearMinusFar),-1.f},
			glm::vec4{0.f,0.f,2.f * near * far / (nearMinusFar),0.f} };
		//glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
		worldToCameraToNDC = cameraToNDC * worldToCamera;

		matrixUpdate = false;
	}
}

glm::mat3 rotationMat(glm::vec3 axis, float angle) {

	const float cosA{ cos(angle) };
	const float sinA{ sin(angle) };
	const float rCosA{ 1 - cosA };
	const float x{ axis.x };
	const float y{ axis.y };
	const float z{ axis.z };
	return glm::mat3{
		glm::vec3{cosA + x * x * rCosA, y * x * rCosA + z * sinA,z * x * rCosA - y * sinA},
		glm::vec3{x * y * rCosA - z * sinA,cosA + y * y * rCosA,z * y * rCosA + x * sinA},
		glm::vec3{x * z * rCosA + y * sinA,y * z * rCosA - x * sinA,cosA + z * z * rCosA}
	};
}

const glm::vec3 Camera::getCameraPos() const
{
	return position;
}