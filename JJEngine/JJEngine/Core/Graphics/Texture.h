/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include<memory>
#include<tuple>
enum class TextureChannel
{
	RGB,
	RGBA,
	RGBA32F,
	Depth,
	None,
};

struct TextureChannelData
{
	unsigned TextureChannelTypeToOpenGLInnerType() const;
	unsigned TextureChannelTypeToOpenGLType() const;
	TextureChannelData(TextureChannel channel):channel(channel) {}
	TextureChannel channel;
};

struct TextureData
{
	TextureData() :width(0), height(0), data(nullptr), channel(TextureChannel::RGBA) {}
	TextureData(int width, int height, 
		std::shared_ptr<unsigned char[]> data = nullptr, TextureChannelData channel = TextureChannel::RGBA)
		:width(width),height(height), channel(channel), data(data) {}
	int width = 0;
	int height = 0;
	std::shared_ptr<unsigned char[]> data{};
	TextureChannelData channel = TextureChannel::RGBA;
};

class Texture
{
public:
	static std::shared_ptr<Texture> CreateTexture(std::shared_ptr<TextureData> texture_data);
	static std::shared_ptr<Texture> CreateTexture(const TextureData& texture_data);
	virtual ~Texture();
	std::tuple<int, int> GetWidthHeight() const;
	unsigned int GetTextureID() const;
	unsigned int GetUnitID() const;

	void BindTexture(unsigned int unit = 0);
	void UnBindTexture();

private:
	Texture(std::shared_ptr<TextureData> texture_data);
	Texture(const TextureData& texture_data);
	int m_Width;
	int m_Height;
	unsigned int m_TextureID;
	unsigned int m_UnitID;
};