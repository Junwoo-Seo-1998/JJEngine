/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "Texture.h"
#include "glad.h"
#include "Core/Utils/Assert.h"

unsigned TextureTargetData::TextureTargetDataToOpenGLType() const
{
	switch (target)
	{
	case TextureTarget::Texture2D: return GL_TEXTURE_2D;
	case TextureTarget::CubeMap: return GL_TEXTURE_CUBE_MAP;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}

unsigned TextureChannelData::TextureChannelTypeToOpenGLInnerType() const
{
	switch (channel)
	{
	case TextureChannel::R_INT:
		return GL_R32I;
	case TextureChannel::RGB:
		return GL_RGB8;
	case TextureChannel::RGB16F:
		return GL_RGB16F;
	case TextureChannel::RGBA:
		return GL_RGBA8;
	case TextureChannel::RGBA32F:
		return GL_RGBA32F;
	case TextureChannel::Depth:
		return GL_DEPTH24_STENCIL8;
	default:
		break;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}

unsigned TextureChannelData::TextureChannelTypeToOpenGLType() const
{
	switch (channel)
	{
	case TextureChannel::R_INT:
		return GL_RED_INTEGER;
	case TextureChannel::RGB:
	case TextureChannel::RGB16F:
		return GL_RGB;
	case TextureChannel::RGBA:
	case TextureChannel::RGBA32F:
		return GL_RGBA;
	case TextureChannel::Depth:
		return GL_DEPTH_STENCIL;
	default:
		break;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}

unsigned TextureChannelData::TextureChannelTypeToOpenGLDataType() const
{
	switch (channel)
	{
	case TextureChannel::R_INT:
		return GL_INT;
	case TextureChannel::RGB:
	case TextureChannel::RGBA:
		return GL_UNSIGNED_BYTE;
	case TextureChannel::RGB16F:
	case TextureChannel::RGBA32F:
		return GL_FLOAT;
	case TextureChannel::Depth:
		return GL_UNSIGNED_INT_24_8;
	default:
		break;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Channel");
	return 0;
}


unsigned TextureWrapData::TextureWrapDataToOpenGLType() const
{
	switch (wrap)
	{
	case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
	case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
	case TextureWrap::Repeat: return GL_REPEAT;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Wrapping Method");
	return 0;
}

unsigned TextureFilterData::TextureFilterDataToOpenGLType() const
{
	switch (filter)
	{
	case TextureFilter::Linear: return GL_LINEAR;
	case TextureFilter::Nearest: return GL_NEAREST;
	}
	ENGINE_ASSERT(false, "Not Supported Texture Filtering Medthod");
	return 0;
}

std::shared_ptr<Texture> Texture::CopyTexture(std::shared_ptr<Texture> texture)
{
	return std::shared_ptr<Texture>(new Texture{ texture });
}

std::shared_ptr<Texture> Texture::CreateTexture(std::shared_ptr<TextureData> texture_data)
{
	return std::shared_ptr<Texture>(new Texture{ texture_data });
}

std::shared_ptr<Texture> Texture::CreateTexture(const TextureData& texture_data)
{
	return std::shared_ptr<Texture>(new Texture{ texture_data });
}

std::shared_ptr<Texture> Texture::CreateTexture(const glm::vec4& color)
{
	auto textureData = std::make_shared<TextureData>();
	auto data = std::shared_ptr<float[]>(new float[4]);
	textureData->data = data;
	textureData->width = 1;
	textureData->height = 1;
	textureData->channel = TextureChannel::RGBA32F;
	textureData->wrap = TextureWrap::ClampToEdge;
	textureData->filter = TextureFilter::Linear;

	data[0] = color.r;
	data[1] = color.g;
	data[2] = color.b;
	data[3] = color.a;

	return std::shared_ptr<Texture>(new Texture{ textureData });
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
}

std::tuple<int, int> Texture::GetWidthHeight() const
{
	return { m_Width, m_Height };
}

unsigned int Texture::GetTextureID() const
{
	return m_TextureID;
}

void Texture::BindTexture(unsigned int unit)
{
	glBindTextureUnit(unit, m_TextureID);
}

void Texture::ClearTexture(int value)
{
	glClearTexImage(m_TextureID, 0, m_TextureChannel.TextureChannelTypeToOpenGLType(), m_TextureChannel.TextureChannelTypeToOpenGLDataType(), &value);
}

Texture::Texture(std::shared_ptr<TextureData> texture_data)
	:m_Width(texture_data->width), m_Height(texture_data->height),
	m_TextureTarget(texture_data->target),
	m_TextureChannel(texture_data->channel),
	m_Wrap(texture_data->wrap), m_Filter(texture_data->filter)
{
	CreateTextureInner(*texture_data);
}

Texture::Texture(const TextureData& texture_data)
	:m_Width(texture_data.width), m_Height(texture_data.height),
	m_TextureTarget(texture_data.target),
	m_TextureChannel(texture_data.channel),
	m_Wrap(texture_data.wrap), m_Filter(texture_data.filter)
{
	CreateTextureInner(texture_data);
}

void Texture::CreateTextureInner(const TextureData& texture_data)
{
	glCreateTextures(texture_data.target.TextureTargetDataToOpenGLType(), 1, &m_TextureID);

	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, m_Wrap.TextureWrapDataToOpenGLType());
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, m_Wrap.TextureWrapDataToOpenGLType());

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, m_Filter.TextureFilterDataToOpenGLType());
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, m_Filter.TextureFilterDataToOpenGLType());

	if (m_Wrap.wrap == TextureWrap::ClampToBorder)
	{
		GLfloat border[] = { 0 };
		glTextureParameterfv(m_TextureID, GL_TEXTURE_BORDER_COLOR, border);
	}

	//jun: allocate this will handle 6 faces too (https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml)
	glTextureStorage2D(m_TextureID, 1, texture_data.channel.TextureChannelTypeToOpenGLInnerType(), m_Width, m_Height);

	//jun: set data
	if (texture_data.data != nullptr)
	{
		if (m_TextureTarget.target == TextureTarget::Texture2D)
		{
			glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height,
				texture_data.channel.TextureChannelTypeToOpenGLType(),
				m_TextureChannel.TextureChannelTypeToOpenGLDataType(), texture_data.data.get());
			glGenerateTextureMipmap(m_TextureID);
		}
		else if(m_TextureTarget.target == TextureTarget::CubeMap)
		{
			//jun: we can use 3D to load cube map(https://www.khronos.org/opengl/wiki/Cubemap_Texture#Cubemap_array_textures)
			float** imagePtrs = (float**)texture_data.data.get();
			for (int i = 0; i < 6; ++i)
			{
				float* imgData = imagePtrs[i];
				glTextureSubImage3D(m_TextureID, 0, 0, 0, i, m_Width, m_Height, 1,
					texture_data.channel.TextureChannelTypeToOpenGLType(),
					m_TextureChannel.TextureChannelTypeToOpenGLDataType(), imgData);
			}
		}
	}
}


Texture::Texture(std::shared_ptr<Texture> texture)
	:m_Width(texture->m_Width), m_Height(texture->m_Height),
	m_TextureTarget(texture->m_TextureTarget),
	m_TextureChannel(texture->m_TextureChannel),
	m_Wrap(texture->m_Wrap), m_Filter(texture->m_Filter)

{
	glCreateTextures(texture->m_TextureTarget.TextureTargetDataToOpenGLType(), 1, &m_TextureID);

	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, m_Wrap.TextureWrapDataToOpenGLType());
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, m_Wrap.TextureWrapDataToOpenGLType());

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, m_Filter.TextureFilterDataToOpenGLType());
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, m_Filter.TextureFilterDataToOpenGLType());

	if (m_Wrap.wrap == TextureWrap::ClampToBorder)
	{
		GLfloat border[] = { 0 };
		glTextureParameterfv(m_TextureID, GL_TEXTURE_BORDER_COLOR, border);
	}

	//jun: allocate this will handle 6 faces too (https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml)
	glTextureStorage2D(m_TextureID, 1, texture->m_TextureChannel.TextureChannelTypeToOpenGLInnerType(), m_Width, m_Height);
	
	//set
	glCopyImageSubData(texture->GetTextureID(),
		texture->m_TextureTarget.TextureTargetDataToOpenGLType(),
		0, 0, 0, 0, m_TextureID,
		texture->m_TextureTarget.TextureTargetDataToOpenGLType(),
		0, 0, 0, 0, m_Width, m_Height, 1);
	glGenerateTextureMipmap(m_TextureID);

}
