#include <glad.h>
#include "HDRIConverter.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/MeshFactory.h"

const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
const glm::mat4 captureViews[] =
{
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};


HDRIConverter::HDRIConverter(int width, int height, std::shared_ptr<Mesh> cube) : width(width), height(height), m_CubemapMesh(cube)
{
	m_HDRItoCubemapShader = Shader::CreateShaderFromFile({
	{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRIConvertShader.vert"}},
	{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRIConvertShader.frag"} }
		});
}

unsigned int HDRIConverter::HDRItoCubemap(std::shared_ptr<Texture> HDRITexture)
{
	unsigned int envCubemap;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLuint captureFBO, captureRBO;
	glCreateFramebuffers(1, &captureFBO);
	glCreateRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glNamedRenderbufferStorage(captureRBO, GL_DEPTH_COMPONENT24, 512, 512);
	glNamedFramebufferRenderbuffer(captureFBO, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &envCubemap);
	glTextureStorage2D(envCubemap, 1, GL_RGB16F, 512, 512);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTextureSubImage2D(envCubemap, 0, 0, 0, 512, 512, GL_RGB, GL_FLOAT, nullptr);
	}

	glTextureParameteri(envCubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(envCubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(envCubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(envCubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(envCubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	m_HDRItoCubemapShader->Use();
	m_HDRItoCubemapShader->SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, HDRITexture->GetTextureID());
	m_HDRItoCubemapShader->SetInt("equirectangularMap", 0);
	glViewport(0, 0, 512, 512);

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_HDRItoCubemapShader->SetMat4("view", captureViews[i]);
		glNamedFramebufferTextureLayer(captureFBO, GL_COLOR_ATTACHMENT0, envCubemap, 0, i);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_CubemapMesh->GetMeshVBO()->BindToVertexArray();
		m_CubemapMesh->GetMeshEBO()->BindToVertexArray();
		glDrawElements(GL_TRIANGLES, m_CubemapMesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	return envCubemap;
}
