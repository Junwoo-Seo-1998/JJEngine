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
void FrameBuffer::Bind(bool clear) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
	for (auto format : m_DescribedFrameBuffer.Formats)
	{
		if (helper::IsDepth(format))
		{
			m_DepthFormat = format;
		}
		else
		{
			m_ColorFormats.emplace_back(format);
		}
	}

	BuildFrameBuffer();
}

void FrameBuffer::BuildFrameBuffer()
{
	if (m_FrameBufferID != 0)
	{
		//remove previous data
		glDeleteFramebuffers(GL_FRAMEBUFFER, &m_FrameBufferID);
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
			const FrameBufferFormat format = m_ColorFormats[i];
			switch (format)
			{
				case FrameBufferFormat::RGB:
				{
					m_ColorTextures[i] = Texture::CreateTexture({ width,height,nullptr,TextureChannel::RGB });
					break;
				}
				case  FrameBufferFormat::RGBA:
				{
					m_ColorTextures[i] = Texture::CreateTexture({width,height,nullptr,TextureChannel::RGBA});
					break;
				}
			}
			unsigned textureID = m_ColorTextures[i]->GetTextureID();
			glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + i, textureID, 0);
		}
	}

	//make depth
	if (m_DepthFormat != FrameBufferFormat::None)
	{
		
		const int width = static_cast<int>(m_DescribedFrameBuffer.Width);
		const int height = static_cast<int>(m_DescribedFrameBuffer.Height);

		switch (m_DepthFormat)
		{
			case FrameBufferFormat::Depth:
			{
				m_DepthTexture = Texture::CreateTexture({ width,height,nullptr, TextureChannel::Depth });
				break;
			}
		}
		unsigned textureID = m_DepthTexture->GetTextureID();
		glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, textureID, 0);
	}


	if (m_ColorFormats.size() > 1)
	{
		const unsigned int size = static_cast<unsigned int>(m_ColorFormats.size());

		ENGINE_ASSERT(size <= 4, "Framebuffer max color attachment size is 4");

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
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
