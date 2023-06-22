#pragma once
#include <memory>

#include "Core/Graphics/Mesh.h"
#include "glm/glm.hpp"
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
	friend Application;
public:
	SceneRenderer();

	void SetScene(Scene* scene);
	void SetViewportSize(unsigned int width, unsigned int height);
	void BeginScene(const glm::mat4& viewProjection, const glm::vec3& camPos);
	void EndScene();

	std::shared_ptr<RenderPass> GetFinalRenderPass();
private:
	void Init();

	//todo change it to shared ptr later 
	Scene* m_ActiveScene = nullptr;
	bool m_Active = false;
	unsigned int m_Width, m_Height;
	bool m_NeedsResize = true;//viewport

	std::shared_ptr<RenderPass> m_FinalRenderPass;
public:
	//�����
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

//begin���� Ŀ�ǵ� Ÿ�� ���ϰ� end���� Ŀ�ǵ� �߰�