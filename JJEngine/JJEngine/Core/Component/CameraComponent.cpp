#include "CameraComponent.h"
#include "Core/Utils/Math/MatrixMath.h"

glm::mat4 CameraComponent::GetProjection() const
{
	return MatrixMath::BuildPerspectiveProjectionMatrixFovy(Fov_y, Aspect_ratio, Near, Far);
}
