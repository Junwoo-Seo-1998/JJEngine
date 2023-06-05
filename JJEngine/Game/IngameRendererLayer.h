#pragma once
#include "Core/Layer/Layer.h"
#include <entt/entt.hpp>
#include <memory>
class Scene;
class VertexArray;
class FrameBuffer;
class FreeCam;


class IngameRendererLayer : public Layer
{
	static constexpr unsigned int MAX_SHADOW_BUFFER = 6;
public:
	virtual ~IngameRendererLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnImGuiRender() override;

private:
	std::shared_ptr<Scene> active_scene;
	std::shared_ptr<VertexArray> renderer_vao;
	std::shared_ptr<FrameBuffer> g_buffer;
	std::shared_ptr<FrameBuffer> shadow_buffer; // maximum is 6.


};
