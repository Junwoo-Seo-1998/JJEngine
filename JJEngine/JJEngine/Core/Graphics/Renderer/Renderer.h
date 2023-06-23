#pragma once
#include <memory>
#include "Core/Command/CommandQueue.h"

class Application;
class RenderPass;
class Texture;
class VertexArray;
class FrameBuffer;

class Renderer
{
	friend Application;
public:
	static std::shared_ptr<Texture> WhiteTexture;
	static std::shared_ptr<Texture> BlackTexture;

	static void BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear = true);
	static void EndRenderPass();

	static void DrawFullScreenQuad();
	

	static void Render();

	template<typename FuncT>
	static void Submit(FuncT&& func)
	{
		auto renderCmd = [](void* ptr) {
			auto pFunc = (FuncT*)ptr;
			(*pFunc)();
			pFunc->~FuncT();
		};
		auto storageBuffer = m_RenderCommandQueue.Allocate(renderCmd, sizeof(func));
		new (storageBuffer) FuncT(std::forward<FuncT>(func));
	}
private:
	static void Init();

	static CommandQueue m_RenderCommandQueue;
};

