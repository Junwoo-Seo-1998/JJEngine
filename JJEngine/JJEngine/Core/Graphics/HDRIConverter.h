#pragma once
#include <memory>
#include "Core/Graphics/Renderer/SceneRenderer.h"
class Texture;
class FrameBuffer;
class Shader;
class HDRIConverter
{
public:
	HDRIConverter(int width, int height, std::shared_ptr<Mesh> cube);
	unsigned int HDRItoCubemap(std::shared_ptr<Texture> HDRITexture);
private:
	int width, height;
	std::shared_ptr<Shader> m_HDRItoCubemapShader;
	std::shared_ptr<Mesh> m_CubemapMesh;

};