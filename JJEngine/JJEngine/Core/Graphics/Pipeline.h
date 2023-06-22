#pragma once
#include <memory>
#include <string>

class VertexArray;
class Shader;
class RenderPass;
struct PipelineSpecification
{
	std::shared_ptr<Shader> Shader;
	std::shared_ptr<RenderPass> RenderPass;
	bool BackfaceCulling = true;
	bool DepthTest = true;
	bool DepthWrite = true;
	bool Wireframe = false;
	float LineWidth = 1.0f;

	std::string DebugName;
};

//아직은 특별한 기능은 없음 나중에 확장성을 위해서 클래스로 존재
class Pipeline
{
public:
	static std::shared_ptr<Pipeline> Create(const PipelineSpecification& spec);

	PipelineSpecification& GetSpecification();
	const PipelineSpecification& GetSpecification() const;
public:
	Pipeline(const PipelineSpecification& spec);
private:
	PipelineSpecification m_Specification;
};
