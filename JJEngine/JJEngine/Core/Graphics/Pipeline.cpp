#include "Pipeline.h"
#include "VertexArray.h"

std::shared_ptr<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
{
	return std::make_shared<Pipeline>(spec);
}

PipelineSpecification& Pipeline::GetSpecification()
{
	return m_Specification;
}

const PipelineSpecification& Pipeline::GetSpecification() const
{
	return m_Specification;
}


Pipeline::Pipeline(const PipelineSpecification& spec)
	:m_Specification(spec)
{
}
