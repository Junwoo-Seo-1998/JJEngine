/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include<memory>
#include<tuple>
#include <glm/vec4.hpp>

enum class TextureChannel
{
	R_INT,
	RGB,
	RGB16F,
	RGBA,
	RGBA32F,
	Depth,
	None,
};

enum class TextureWrap
{
	None = 0,
	ClampToEdge,
	ClampToBorder,
	Repeat
};

enum class TextureFilter
{
	None = 0,
	Linear,
	Nearest,
};

struct TextureChannelData
{
	unsigned TextureChannelTypeToOpenGLInnerType() const;
	unsigned TextureChannelTypeToOpenGLType() const;
	unsigned TextureChannelTypeToOpenGLDataType() const;
	TextureChannelData(TextureChannel channel):channel(channel) {}
	TextureChannel channel;
};

struct TextureWrapData
{
	unsigned TextureWrapDataToOpenGLType() const;
	TextureWrapData(TextureWrap wrap) :wrap(wrap) {}
	TextureWrap wrap;
};

struct TextureFilterData
{
	unsigned TextureFilterDataToOpenGLType() const;
	TextureFilterData(TextureFilter filter) :filter(filter) {}
	TextureFilter filter;
};

struct TextureData
{
	TextureData() :width(0), height(0), data(nullptr), channel(TextureChannel::RGBA) {}
	TextureData(int width, int height, 
		std::shared_ptr<void> data = nullptr, TextureChannelData channel = TextureChannel::RGBA)
		:width(width),height(height), channel(channel), data(data) {}
	int width = 0;
	int height = 0;
	std::shared_ptr<void> data{};
	TextureChannelData channel = TextureChannel::RGBA;
	TextureWrapData wrap = TextureWrap::ClampToEdge;
	TextureFilter filter = TextureFilter::Linear;
};

class Texture
{
public:
	static std::shared_ptr<Texture> CopyTexture(std::shared_ptr<Texture> texture);
	static std::shared_ptr<Texture> CreateTexture(std::shared_ptr<TextureData> texture_data);
	static std::shared_ptr<Texture> CreateTexture(const TextureData& texture_data);
	static std::shared_ptr<Texture> CreateTexture(const glm::vec4& color);
	virtual ~Texture();
	std::tuple<int, int> GetWidthHeight() const;
	unsigned int GetTextureID() const;
	unsigned int GetUnitID() const;

	void BindTexture(unsigned int unit = 0);
	void UnBindTexture();

	//clear texture with the value
	void ClearTexture(int value = -1);
private:
	Texture(std::shared_ptr<TextureData> texture_data);
	Texture(const TextureData& texture_data);
	Texture(std::shared_ptr<Texture> texture_data);

	int m_Width;
	int m_Height;
	TextureChannelData m_TextureChannel;
	TextureWrapData m_Wrap;
	TextureFilterData m_Filter;
	unsigned int m_TextureID;
	unsigned int m_UnitID;

};