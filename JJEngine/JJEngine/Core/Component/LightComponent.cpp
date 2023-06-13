#include "LightComponent.h"
#include "Core/Utils/Math/MatrixMath.h"
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 LightComponent::GetProjection() const
{
	//return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	return MatrixMath::BuildPerspectiveProjectionMatrixFovy(Fov_y, Aspect_ratio, Near, Far);
}
