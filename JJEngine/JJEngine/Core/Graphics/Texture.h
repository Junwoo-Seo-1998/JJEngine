#pragma once
#include<memory>
struct TextureData
{
	int width = 0;
	int height = 0;
	int number_of_channels = 0;
	std::shared_ptr<unsigned char[]> data{};
};