#include "RenderPass.h"
#include "FrameBuffer.h"

RenderPass::RenderPass(const RenderPassSpecification& spec)
	:m_RenderPassSpecification(spec)
{}

RenderPassSpecification& RenderPass::GetSpecification()
{
	return m_RenderPassSpecification;
}

const RenderPassSpecification& RenderPass::GetSpecification() const
{
	return m_RenderPassSpecification;
}

std::shared_ptr<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
{
	return std::make_shared<RenderPass>(spec);
}


