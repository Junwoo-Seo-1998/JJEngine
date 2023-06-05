#pragma once
#include <memory>
#include <glm/glm.hpp>
//(jun) note this class is for testing so it will be changed in future

class Texture;
struct SpriteRendererComponent
{
	SpriteRendererComponent() = default;
	SpriteRendererComponent(std::shared_ptr<Texture> texture):texture(texture){}
	SpriteRendererComponent(const glm::vec4& color):color(color) {}
	std::shared_ptr<Texture> texture;
	glm::vec4 color{ 1.f, 1.f ,1.f, 1.f };
};
