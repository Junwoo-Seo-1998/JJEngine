#pragma once
#include <vector>
#include "Texture.h"
enum class FrameBufferFormat
{
	None = 0,
	RGB,
	RGBA,
	RGBA32F,
	Depth,
};

struct DescribedFrameBufferFormats
{
	DescribedFrameBufferFormats() = default;
	DescribedFrameBufferFormats(std::initializer_list<FrameBufferFormat> formats)
		:Formats(formats) {}

	std::vector<FrameBufferFormat>::iterator begin() { return Formats.begin(); }
	std::vector<FrameBufferFormat>::iterator end() { return Formats.end(); }

	std::vector<FrameBufferFormat> Formats;
};

struct FrameBufferSpecification
{
	unsigned int Width, Height;
	DescribedFrameBufferFormats Formats;
};

class FrameBuffer
{
public:
	static std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec);
	FrameBuffer() = delete;
	virtual ~FrameBuffer();

	void Bind(bool clear = false) const;
	void UnBind() const;

	void Resize(unsigned int width, unsigned int height);

	unsigned int GetFrameBufferID() const;
	std::shared_ptr<Texture> GetColorTexture(int index);
	std::shared_ptr<Texture> GetDepthTexture();

	int GetPixelInt(int colorTextureIndex, int x, int y);

	FrameBufferSpecification GetSpecification() const;
private:
	FrameBuffer(const FrameBufferSpecification& spec);
	void BuildFrameBuffer();
	FrameBufferSpecification m_DescribedFrameBuffer;
	std::vector<FrameBufferFormat> m_ColorFormats;
	FrameBufferFormat m_DepthFormat;
	
	unsigned int m_FrameBufferID;
	std::vector<std::shared_ptr<Texture>> m_ColorTextures;
	std::shared_ptr<Texture> m_DepthTexture;
};