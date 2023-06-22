#pragma once
#include <memory>

#include "Core/Graphics/Mesh.h"
#include "glm/glm.hpp"
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



	//�����
	static void BeginScene(const glm::mat4& viewProjection, const glm::vec3& pos);
	static void AddModel(const Model& model, const TransformComponent& transform, const MaterialComponent& material);
	static void AddAffectLight(const LightComponent& light, TransformComponent lightTransform);
	static void SetVAO(std::shared_ptr<VertexArray> VAO);
	static void SetGBuffer(std::shared_ptr<FrameBuffer> FBO, Mesh FSQ);
	static void SetShadowBuffer(const std::shared_ptr<FrameBuffer>& FBO);
	static void SetShadowInformation(glm::ivec2 resolution, glm::ivec2 zOffset);
	static void EndScene();
	static void DrawAllScene();




	static std::shared_ptr<RenderPass> GetFinalRenderPass();
};

//begin���� Ŀ�ǵ� Ÿ�� ���ϰ� end���� Ŀ�ǵ� �߰�