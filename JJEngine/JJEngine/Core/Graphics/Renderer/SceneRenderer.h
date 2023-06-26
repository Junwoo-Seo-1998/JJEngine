#pragma once
#include <memory>

#include "Core/Graphics/Light.h"
#include "Core/Graphics/Mesh.h"
#include "glm/glm.hpp"
class Shader;
class Scene;
class Application;
class FrameBuffer;
class VertexArray;
struct MaterialComponent;
struct TransformComponent;
struct LightComponent;
class Model;
class RenderPass;
class Texture;


class SceneRenderer
{
public:
	SceneRenderer();

	void SetScene(Scene* scene);
	void SetViewportSize(unsigned int width, unsigned int height);
	void BeginScene(const glm::mat4& view, const glm::mat4& Projection, const glm::vec3& camPos);
	void EndScene();

	//todo: implement with material instance (not material, material instance)
	void SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transformMat);

	std::shared_ptr<RenderPass> GetFinalRenderPass();
private:
	void Init();

	bool isFirst = true;
	void HDRItoCubemapPass();

	void CubemapPass();

	void GeometryPass();

	void GeometryPassFSQ();

	void ForwardPass();


	//post processing
	void BloomPass();
	void HDRPass();

	void DebugRenderingPass();

	//todo change it to shared ptr later 
	Scene* m_ActiveScene = nullptr;

	bool m_Active = false;
	unsigned int m_Width, m_Height;
	bool m_NeedsResize = true;//viewport

	std::shared_ptr<VertexArray> m_VertexArray;

	//just for now it will be removed later
	//view projection
	//camPos
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::vec3 m_CameraPosition;

	//scene data
	std::vector<std::tuple<glm::vec3, glm::vec3, Light>> m_ActiveLights;


	//Default Shader
	std::shared_ptr<Material> m_DefaultMaterial;
	std::shared_ptr<Shader> m_NormalRenderShader;

	std::shared_ptr<Shader> m_GeometryShader;
	std::shared_ptr<Shader> m_FinalRenderShader;

	//post processing
	std::shared_ptr<Shader> m_BloomExtractShader;
	std::shared_ptr<Shader> m_BloomBlurShader;
	std::shared_ptr<Shader> m_BloomRenderShader;
	std::shared_ptr<Shader> m_HDRRenderShader;

	std::shared_ptr<RenderPass> m_GeometryRenderPass;
	std::shared_ptr<RenderPass> m_FinalRenderPass;
	std::shared_ptr<RenderPass> m_BloomExtractRenderPass;
	std::shared_ptr<RenderPass> m_BloomBlurRenderPass[2];

	//Cubemap
	std::shared_ptr<Shader> m_CubemapShader;
	std::shared_ptr<Mesh> m_CubemapMesh;

	//HDRI to Cubemap

	unsigned int envCubemap;
	std::shared_ptr<Texture> m_HDRICubemapTexture;
	std::shared_ptr<RenderPass> m_HDRItoCubemapRenderPass;
	std::shared_ptr<Shader> m_HDRItoCubemapShader;


	struct DrawCommand
	{
		std::shared_ptr<Mesh> Mesh;
		glm::mat4 Transform;
	};
	std::vector<DrawCommand> m_GeometryDrawList;
	std::vector<DrawCommand> m_DrawList;


};
