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

enum class TextureTarget
{
	Texture2D,
	CubeMap,
};

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

struct TextureTargetData
{
	unsigned TextureTargetDataToOpenGLType() const;
	TextureTargetData(TextureTarget target) :target(target) {}
	TextureTarget target;
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
		std::shared_ptr<void> data = nullptr,
		TextureTarget target = TextureTarget::Texture2D,
		TextureChannel channel = TextureChannel::RGBA,
		TextureWrap wrap = TextureWrap::ClampToEdge,
		TextureFilter filter = TextureFilter::Linear)
		:width(width), height(height), data(data),
		target(target), channel(channel), wrap(wrap), filter(filter) {}

	int width = 0;
	int height = 0;
	std::shared_ptr<void> data{};

	TextureTargetData target = TextureTarget::Texture2D;
	TextureChannelData channel = TextureChannel::RGBA;
	TextureWrapData wrap = TextureWrap::ClampToEdge;
	TextureFilterData filter = TextureFilter::Linear;
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

	void BindTexture(unsigned int unit = 0);

	//clear texture with the value
	void ClearTexture(int value = -1);
private:
	Texture(std::shared_ptr<TextureData> texture_data);
	Texture(const TextureData& texture_data);
	void CreateTextureInner(const TextureData& texture_data);

	Texture(std::shared_ptr<Texture> texture_data);

	int m_Width;
	int m_Height;
	TextureTargetData m_TextureTarget;
	TextureChannelData m_TextureChannel;
	TextureWrapData m_Wrap;
	TextureFilterData m_Filter;
	unsigned int m_TextureID;
};