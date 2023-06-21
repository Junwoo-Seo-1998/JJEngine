#pragma once
#include <memory>
#include <string>
class FrameBuffer;
struct RenderPassSpecification
{
	std::shared_ptr<FrameBuffer> TargetFramebuffer;
	std::string DebugName;
};

//this is used to know where to draw simple wrapper class of framebuffer class
class RenderPass
{
public:
	~RenderPass() = default;
	RenderPass(const RenderPassSpecification& spec);
	RenderPassSpecification& GetSpecification();
	const RenderPassSpecification& GetSpecification() const;
	static std::shared_ptr<RenderPass> Create(const RenderPassSpecification& spec);
private:
	RenderPassSpecification m_RenderPassSpecification;
};