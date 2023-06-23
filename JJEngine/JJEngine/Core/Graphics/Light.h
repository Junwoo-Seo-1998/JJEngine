#pragma once
#include "glm/glm.hpp"

enum class LightType
{
	PointLight = 0,
	DirectionLight,
	SpotLight
};

struct Light
{
	LightType m_LightType{ LightType::PointLight };
	struct
	{
		float inner = 30.f;
		float outer = 45.f;
	} m_Angle;

	glm::vec3 Ambient{0.f};
	glm::vec3 Diffuse{0.5f};
	glm::vec3 Specular{0.2f};
	float falloff{ 32.f };
};