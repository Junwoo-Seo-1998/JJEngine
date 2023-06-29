#include "LightComponent.h"
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 LightComponent::GetProjection() const
{
	//return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	switch (light.m_LightType)
	{
		case LightType::DirectionLight:
		{
			return glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 500.f);
		}
		default:
		{
			return glm::ortho(-35.f, 35.f, -35.f, 35.f, 0.1f, 500.f);
		}
	}
}
