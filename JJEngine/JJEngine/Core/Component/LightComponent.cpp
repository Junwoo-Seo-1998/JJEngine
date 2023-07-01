#include "LightComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Utils/Math/MatrixMath.h"
glm::mat4 LightComponent::GetProjection() const
{
	//return glm::frustum(-0.6f, 0.6f, -0.6f, 0.6f, 0.1f, 500.f);
	//return MatrixMath::BuildPerspectiveProjectionMatrixFovy(Fov_y, Aspect_ratio, Near, Far);

	
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
