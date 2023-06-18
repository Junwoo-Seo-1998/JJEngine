#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "entt/entt.hpp"
#include "Core/Component/TransformComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/CameraComponent.h"

#include "Core/Graphics/Mesh.h"

class Texture;
class VertexArray;
class FrameBuffer;

class Renderer
{
public:
	static void BeginScene(const glm::mat4& viewProjection, const glm::vec3& pos);
	static void AddModel(const Model& model, const TransformComponent& transform, const MaterialComponent& material);
	static void AddAffectLight(const LightComponent& light, TransformComponent lightTransform);
	static void SetVAO(std::shared_ptr<VertexArray> VAO);
	static void SetGBuffer(std::shared_ptr<FrameBuffer> FBO, Mesh FSQ);
	static void SetShadowBuffer (const std::shared_ptr<FrameBuffer>& FBO);
	static void SetShadowInformation(glm::ivec2 resolution, glm::ivec2 zOffset);
	static void EndScene();
	static void DrawAllScene();
};

//begin에서 커맨드 타입 정하고 end에서 커맨드 추가