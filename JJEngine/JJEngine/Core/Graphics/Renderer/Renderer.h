#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "entt/entt.hpp"
#include "Core/Graphics/Graphics.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Component/LightComponent.h"

class Texture;


class Renderer
{
public:
	static void BeginScene(const glm::mat4& viewProjection);
	static void SetModel(const Model& model);
	static void AddAffectLight(const LightComponent& light, TransformComponent lightTransform);
	static void SetTransform(const TransformComponent& transform);
	static void SetMaterial(const MaterialComponent& material);
	static void SetVAO(std::shared_ptr<VertexArray> VAO);
	static void SetGBuffer(std::shared_ptr<FrameBuffer> FBO);
	static void SetShadowBuffer (const std::shared_ptr<FrameBuffer>& FBO);
	static void EndScene();

};

//begin에서 커맨드 타입 정하고 end에서 커맨드 추가