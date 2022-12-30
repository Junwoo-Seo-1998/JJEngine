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

};

class TextureChannelData
{
	unsigned ShaderDataTypeToOpenGLBaseType() const;
};

struct TextureData
{
	int width = 0;
	int height = 0;
	int number_of_channels = 0;
	std::shared_ptr<unsigned char[]> data{};
};

class Texture
{
public:
	static std::shared_ptr<Texture> CreateTexture(std::shared_ptr<TextureData> texture_data);
	virtual ~Texture();
	std::tuple<int, int> GetWidthHeight() const;
	unsigned int GetTextureID() const;
	void BindTexture(unsigned int unit = 0);
private:
	Texture(std::shared_ptr<TextureData> texture_data);
	int m_Width;
	int m_Height;
	unsigned int m_TextureID;
};