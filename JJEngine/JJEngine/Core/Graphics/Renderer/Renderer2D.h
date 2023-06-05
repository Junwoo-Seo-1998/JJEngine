#pragma once
#include <glm/glm.hpp>
#include <memory>
class Texture;
class EditorCamera;
class Renderer2D
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const glm::mat4& viewProjection);
	static void BeginScene(const EditorCamera& camera);
	static void EndScene();

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
	static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
};
