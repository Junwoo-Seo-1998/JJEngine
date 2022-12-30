/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "Texture.h"
#include "glad.h"
std::shared_ptr<Texture> Texture::CreateTexture(std::shared_ptr<TextureData> texture_data)
{
	return std::shared_ptr<Texture>(new Texture{ texture_data });
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

Texture::Texture(std::shared_ptr<TextureData> texture_data)
	:m_Width(texture_data->width), m_Height(texture_data->height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
	
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(m_TextureID, 1, GL_RGBA8, m_Width, m_Height);
	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, texture_data->data.get());
	glGenerateTextureMipmap(m_TextureID);
}