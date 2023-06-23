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

	void GeometryPass();

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
	std::shared_ptr<Shader> m_GeometryShader;
	std::shared_ptr<Shader> m_FinalRenderShader;

	std::shared_ptr<RenderPass> m_GeometryRenderPass;
	std::shared_ptr<RenderPass> m_FinalRenderPass;

	struct DrawCommand
	{
		std::shared_ptr<Mesh> Mesh;
		glm::mat4 Transform;
	};
	std::vector<DrawCommand> m_DrawList;
public:
	//지울거
	static void BeginSceneCommand(const glm::mat4& viewProjection, const glm::vec3& pos);
	static void AddModel(const Model& model, const TransformComponent& transform, const MaterialComponent& material);
	static void AddAffectLight(const LightComponent& light, TransformComponent lightTransform);
	static void SetVAO(std::shared_ptr<VertexArray> VAO);
	static void SetGBuffer(std::shared_ptr<FrameBuffer> FBO, Mesh FSQ);
	static void SetShadowBuffer(const std::shared_ptr<FrameBuffer>& FBO);
	static void SetShadowInformation(glm::ivec2 resolution, glm::ivec2 zOffset);
	static void EndSceneCommand();
	static void DrawAllScene();


};

//begin에서 커맨드 타입 정하고 end에서 커맨드 추가