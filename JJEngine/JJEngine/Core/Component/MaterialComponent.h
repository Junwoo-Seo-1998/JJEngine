#pragma once
#include <memory>
class Shader;
class Texture;
enum class MaterialType
{
	Forward, Deffered
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
};