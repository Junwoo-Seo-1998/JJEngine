#include "CameraComponent.h"
#include "Core/Utils/Math/MatrixMath.h"

glm::mat4 CameraComponent::GetMatrix() const
{
	return MatrixMath::BuildPerspectiveProjectionMatrixFovy(glm::radians(Fov_y), Aspect_ratio, Near, Far);
}
