#pragma once
#include <glm/glm.hpp>
//for not editor use only
class CameraBase
{
public:
	CameraBase() = default;
	CameraBase(const glm::mat4& projection)
		:m_Projection(projection) {}
	virtual ~CameraBase() = default;

	const glm::mat4& GetProjection() const { return m_Projection; }
protected:
	glm::mat4 m_Projection = glm::mat4{ 1.f };
};
