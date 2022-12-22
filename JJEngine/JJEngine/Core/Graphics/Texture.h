/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include<memory>
struct TextureData
{
	int width = 0;
	int height = 0;
	int number_of_channels = 0;
	std::shared_ptr<unsigned char[]> data{};
};