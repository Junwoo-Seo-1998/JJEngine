#pragma once
#include <vector>
#include <glm/vec4.hpp>

#include "Texture.h"
enum class FrameBufferFormat
{
	None = 0,
	R_INT,
	RGB,
	RGB16F,
	RGBA,
	RGBA32F,
	CubeMap16F,
	Depth,
};

struct FrameBufferFormatSpecification
{
	FrameBufferFormatSpecification(FrameBufferFormat formatType, 
		TextureWrap wrap = TextureWrap::ClampToEdge, 
		TextureFilter filter = TextureFilter::Linear)
		:FormatType(formatType), Wrap(wrap), Filter(filter){}

	FrameBufferFormat FormatType;
	TextureWrap Wrap;
	TextureFilter Filter;
};

struct DescribedFrameBufferFormats
{
	DescribedFrameBufferFormats() = default;

	DescribedFrameBufferFormats(std::initializer_list<FrameBufferFormatSpecification> formats)
		:Formats(formats) {}

	std::vector<FrameBufferFormatSpecification>::iterator begin() { return Formats.begin(); }
	std::vector<FrameBufferFormatSpecification>::iterator end() { return Formats.end(); }

	std::vector<FrameBufferFormatSpecification> Formats;
};

struct FrameBufferSpecification
{
	unsigned int Width, Height;
	DescribedFrameBufferFormats Formats;

	glm::vec4 ClearColor = { 1.f, 1.f,1.f,1.f };
};

class FrameBuffer
{
public:
	static std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FrameBufferSpecification& spec);
	FrameBuffer() = delete;
	virtual ~FrameBuffer();

	void Bind() const;
	void UnBind() const;

	void Resize(unsigned int width, unsigned int height);
	void ChangeCubeMapTextureFace(int color_index, int face_index);
	unsigned int GetFrameBufferID() const;
	std::shared_ptr<Texture> GetColorTexture(int index);
	std::shared_ptr<Texture> GetDepthTexture();

	int GetPixelInt(int colorTextureIndex, int x, int y);

	FrameBufferSpecification GetSpecification() const;
private:
	FrameBuffer(const FrameBufferSpecification& spec);
	void BuildFrameBuffer();
	FrameBufferSpecification m_DescribedFrameBuffer;
	std::vector<FrameBufferFormatSpecification> m_ColorFormats;
	FrameBufferFormatSpecification m_DepthFormat;
	
	unsigned int m_FrameBufferID;
	std::vector<std::shared_ptr<Texture>> m_ColorTextures;
	std::shared_ptr<Texture> m_DepthTexture;
};