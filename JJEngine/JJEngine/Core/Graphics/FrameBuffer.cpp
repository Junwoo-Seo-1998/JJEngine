#include "FrameBuffer.h"
#include <glad.h>
#include "Core/Utils/Assert.h"
namespace helper
{
	static bool IsDepth(FrameBufferFormat format)
	{
		switch (format)
		{
		case FrameBufferFormat::Depth:
			return true;
		}
		return false;
	}
}
std::shared_ptr<FrameBuffer> FrameBuffer::CreateFrameBuffer(const FrameBufferSpecification& spec)
{
	return std::shared_ptr<FrameBuffer>(new FrameBuffer{ spec });
}
FrameBuffer::~FrameBuffer()
{
	UnBind();
	glDeleteFramebuffers(1, &m_FrameBufferID);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glViewport(0, 0, static_cast<int>(m_DescribedFrameBuffer.Width), static_cast<int>(m_DescribedFrameBuffer.Height));
}

void FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	//ignore to set size as zero
	if (width == 0 || height == 0)
	{
		return;
	}
	m_DescribedFrameBuffer.Width = width;
	m_DescribedFrameBuffer.Height = height;
	BuildFrameBuffer();
}

void FrameBuffer::ChangeCubeMapTextureFace(int color_index, int face_index)
{
	ENGINE_ASSERT(m_ColorFormats[color_index].FormatType == FrameBufferFormat::CubeMap16F, "Not a CubeMap!");
	glNamedFramebufferTextureLayer(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + color_index, m_ColorTextures[color_index]->GetTextureID(), 0, face_index);
}

unsigned int FrameBuffer::GetFrameBufferID() const
{
	return m_FrameBufferID;
}

std::shared_ptr<Texture> FrameBuffer::GetColorTexture(int index)
{
	return m_ColorTextures[index];
}

std::shared_ptr<Texture> FrameBuffer::GetDepthTexture()
{
	return m_DepthTexture;
}

int FrameBuffer::GetPixelInt(int colorTextureIndex, int x, int y)
{
	ENGINE_ASSERT(colorTextureIndex < m_ColorTextures.size());

	int val = 0;
	glReadBuffer(GL_COLOR_ATTACHMENT0 + colorTextureIndex);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &val);

	return val;
}

FrameBufferSpecification FrameBuffer::GetSpecification() const
{
	return m_DescribedFrameBuffer;
}

FrameBuffer::FrameBuffer(const FrameBufferSpecification& spec)
	:m_DescribedFrameBuffer(spec), m_DepthFormat(FrameBufferFormat::None)
	, m_FrameBufferID(0), m_ColorFormats(), m_DepthTexture(nullptr)
{

	ENGINE_ASSERT(m_DescribedFrameBuffer.Formats.Formats.empty() != true, "there is no described Format in FrameBuffer!");

	//todo: not using color format now... might use for docking? 
	for (auto& format : m_DescribedFrameBuffer.Formats)
	{
		if (helper::IsDepth(format.FormatType))
		{
			m_DepthFormat = format.FormatType;
		}
		else
		{
			m_ColorFormats.emplace_back(format.FormatType);
		}
	}

	BuildFrameBuffer();
}

void FrameBuffer::BuildFrameBuffer()
{
	if (m_FrameBufferID != 0)
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		//remove previous data
		m_ColorTextures.clear();
		m_DepthTexture.reset();
	}

	glCreateFramebuffers(1, &m_FrameBufferID);

	if (m_ColorFormats.empty() == false)
	{
		const unsigned num_of_textures = static_cast<unsigned int>(m_ColorFormats.size());
		m_ColorTextures.resize(num_of_textures);

		const int width = static_cast<int>(m_DescribedFrameBuffer.Width);
		const int height = static_cast<int>(m_DescribedFrameBuffer.Height);
		for (unsigned int i = 0; i < num_of_textures; ++i)
		{
			const TextureWrap wrap = m_ColorFormats[i].Wrap;
			const TextureFilter filter = m_ColorFormats[i].Filter;
			switch (m_ColorFormats[i].FormatType)
			{
				case FrameBufferFormat::R_INT:
				{
					m_ColorTextures[i] = Texture::CreateTexture(TextureData{ width, height, nullptr, TextureTarget::Texture2D, TextureChannel::R_INT,
						wrap, filter });
					break;
				}
				case FrameBufferFormat::RGB:
				{
					m_ColorTextures[i] = Texture::CreateTexture(TextureData{ width,height,nullptr, TextureTarget::Texture2D, TextureChannel::RGB,
						wrap, filter });
					break;
				}
				case  FrameBufferFormat::RGB16F:
				{
					m_ColorTextures[i] = Texture::CreateTexture(TextureData{ width,height,nullptr, TextureTarget::Texture2D, TextureChannel::RGB16F,
						wrap, filter });
					break;
				}
				case  FrameBufferFormat::RGBA:
				{
					m_ColorTextures[i] = Texture::CreateTexture(TextureData{ width,height,nullptr, TextureTarget::Texture2D, TextureChannel::RGBA,
						wrap, filter });
					break;
				}
				case  FrameBufferFormat::RGBA32F:
				{
					m_ColorTextures[i] = Texture::CreateTexture(TextureData{ width,height,nullptr, TextureTarget::Texture2D, TextureChannel::RGBA32F,
						wrap, filter });
					break;
				}
				case FrameBufferFormat::CubeMap16F:
				{
					TextureData textureSpec{ width, height,nullptr,  TextureTarget::CubeMap, TextureChannel::RGB16F,
						wrap, filter };
					m_ColorTextures[i] = Texture::CreateTexture(textureSpec);
					break;
				}
				default:
					ENGINE_ASSERT(true, "Not Supported Type!");
					break;
			}
			unsigned textureID = m_ColorTextures[i]->GetTextureID();
			if (m_ColorFormats[i].FormatType == FrameBufferFormat::CubeMap16F)
			{
				glNamedFramebufferTextureLayer(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + i, textureID, 0, 0);
			}
			else
			{
				glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + i, textureID, 0);
			}
		}
	}

	//make depth
	if (m_DepthFormat.FormatType != FrameBufferFormat::None)
	{
		
		const int width = static_cast<int>(m_DescribedFrameBuffer.Width);
		const int height = static_cast<int>(m_DescribedFrameBuffer.Height);

		switch (m_DepthFormat.FormatType)
		{
			case FrameBufferFormat::Depth:
			{
				m_DepthTexture = Texture::CreateTexture(TextureData{ 
					width,height,nullptr, TextureTarget::Texture2D, TextureChannel::Depth,
					m_DepthFormat.Wrap, m_DepthFormat.Filter
				});
				break;
			}
			default:
				ENGINE_ASSERT(true, "Not Supported Type!");
				break;
		}
		unsigned textureID = m_DepthTexture->GetTextureID();
		glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, textureID, 0);
	}


	if (m_ColorFormats.size()>=1)
	{
		const unsigned int size = static_cast<unsigned int>(m_ColorFormats.size());

		ENGINE_ASSERT(size <= 8, "Framebuffer max color attachment size is 8");

		GLenum buffers[8] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
		glNamedFramebufferDrawBuffers(m_FrameBufferID, size, buffers);
	}
	else if (m_ColorFormats.empty())//means only depth
	{
		glNamedFramebufferDrawBuffer(m_FrameBufferID, GL_NONE);
		glNamedFramebufferReadBuffer(m_FrameBufferID, GL_NONE);
	}

	if (glCheckNamedFramebufferStatus(m_FrameBufferID,GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EngineLog::Critical("Couldn't make FrameBuffer!");
	}
}
