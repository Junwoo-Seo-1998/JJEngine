#pragma once
#include <memory>
class Shader;
class Texture;
enum class MaterialType
{
	Forward, Deferred
};

struct MaterialComponent
{
	std::shared_ptr<Shader> forwardShader;
	std::shared_ptr<Shader> defferedFirstPassShader;
	std::shared_ptr<Shader> defferedSecondPassShader;
	std::shared_ptr<Shader> shadowSamplingShader;
	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> specularTexture;
	bool isShadowed = true;
	MaterialType type;

	bool operator==(const MaterialComponent& right)
	{
		return type == right.type && isShadowed == right.isShadowed;
	}
};