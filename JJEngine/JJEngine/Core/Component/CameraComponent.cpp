#include "CameraComponent.h"
#include "Core/Utils/Math/MatrixMath.h"

glm::mat4 CameraComponent::GetPerspective() const
{
	return MatrixMath::BuildPerspectiveProjectionMatrixFovy(glm::radians(fov_y), aspect_ratio, near, far);
}
