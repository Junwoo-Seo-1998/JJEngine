#pragma once
#include <memory>
#include <glm/glm.hpp>
//(jun) note this class is for testing so it will be changed in future

class Asset_Texture;
struct SpriteRendererComponent
{
	SpriteRendererComponent() = default;
	SpriteRendererComponent(std::shared_ptr<Asset_Texture> texture):asset(texture){}
	SpriteRendererComponent(const glm::vec4& color):color(color) {}
	std::shared_ptr<Asset_Texture> asset;
	glm::vec4 color{ 1.f, 1.f ,1.f, 1.f };
};
