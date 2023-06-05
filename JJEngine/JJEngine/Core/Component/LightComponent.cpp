#include "LightComponent.h"
#include "Core/Utils/Math/MatrixMath.h"

glm::mat4 LightComponent::GetMatrix() const
{
	return MatrixMath::BuildPerspectiveProjectionMatrixFovy(glm::radians(fov_y), aspect_ratio, near, far);
}
