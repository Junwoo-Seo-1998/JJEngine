#pragma once
#include "glm/glm.hpp"

enum class LightType : int
{
	PointLight = 0,
	DirectionLight = 1,
	SpotLight = 2
};

struct Light
{
	LightType m_LightType{ LightType::PointLight };
	struct
	{
		float inner = 30.f;
		float outer = 45.f;
	} m_Angle;

	glm::vec3 Ambient{1.0f};
	glm::vec3 Diffuse{1.0f};
	glm::vec3 Specular{1.0f};
	float falloff{ 32.f };

	Light() = default;
	Light(const Light& light) = default;
};