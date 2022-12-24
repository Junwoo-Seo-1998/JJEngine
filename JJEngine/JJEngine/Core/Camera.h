/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include "glm/glm.hpp"

class Camera {
	glm::vec3 position{ 0,0,0 };
	glm::vec3 upDirection{ 0,1,0 };
	glm::vec3 rightDirection{ 1,0,0 };
	glm::vec3 backDirection{ 0,0,1 };

	glm::mat4 worldToCamera{};
	glm::mat4 cameraToWorld{};
	glm::mat4 cameraToNDC{};
	glm::mat4 worldToCameraToNDC{};

	bool matrixUpdate{ true };
	float fov{ 0.33f * 3.14f };
	float height{ 800.f };
	float width{ 1200.f };
	float ar{ width / height };
	float near{ 0.1f };
	float far{ 1000.f };
	float distance{ (near + far) * 0.5f };

	glm::vec3 CameraAngle{};

	void WHUpdate();
	void matrixUpdateFunc();

public:
	// Todo: projections
	Camera(float Fov = 1.4f, float AR = 1.5f, float Distance = 500.f, float Near = 0.1f, float Far = 1000.f);
	void Update(double dt);
	void resetAngle();
	
	const glm::mat4 getWorldToCameraToNDC()const { return worldToCameraToNDC; }
	const glm::mat4 getWorldToCamera()const { return worldToCamera; }
	const glm::mat4 getCameraToNDC()const { return cameraToNDC; }
	const glm::mat4 getCameraToWorld()const { return cameraToWorld; }

	const glm::vec3 getCameraPos()const;
	const float getCameraWidth()const { return width; }
	const float getCameraHeight()const { return height; }
	float& getNear() { return near; }
	float& getFar() { return far; }
	float& getFov() { return fov; }
	float& getAr() { return ar; }
	float getheight() { return height; }
	float getFarWidth() { return width / distance * far; }
	glm::vec3 getViewDirec() { return backDirection * -1.f; }
	glm::vec3 getAngle();

	void setPosition(glm::vec3 pos) { position = pos; matrixUpdate = true; }
	void setDistance(float dist) { distance = dist; matrixUpdate = true; }
	void setNear(float Near) { near = Near; matrixUpdate = true; }
	void setFar(float Far) { far = Far; matrixUpdate = true; }
	void setFov(float Fov) { fov = Fov; matrixUpdate = true; }
	void setAr(float Ar) { ar = Ar; matrixUpdate = true; }
	void setIsUpdate(bool TF) { matrixUpdate = TF; };
	void setRotate(glm::vec3 angle);
	void updateRotate(glm::vec3 angle);

	void setLookAt(glm::vec3 lookAt);
	void setLookAt(glm::vec3 lookAtVec, glm::vec3 rightVec, glm::vec3 upVec);
};
glm::mat3 rotationMat(glm::vec3 axis, float angle);
