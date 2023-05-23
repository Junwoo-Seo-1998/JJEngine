#pragma once
#include <memory>

//(jun) note this class is for testing so it will be changed in future

class Texture;
struct SpriteRendererComponent
{
	SpriteRendererComponent() = default;
	SpriteRendererComponent(std::shared_ptr<Texture> texture):texture(texture){}
	std::shared_ptr<Texture> texture;
};
