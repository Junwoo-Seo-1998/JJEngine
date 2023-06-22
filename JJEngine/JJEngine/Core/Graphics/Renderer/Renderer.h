#pragma once
#include <memory>

class RenderPass;
class Texture;
class VertexArray;
class FrameBuffer;

//어느 곳에 그릴지
class Renderer
{
public:
	static void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear = true);
	static void EndRenderPass();
};

