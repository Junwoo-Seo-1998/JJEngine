#pragma once
#include <memory>
#include <string>

class VertexArray;
class Shader;
class RenderPass;
struct PipelineSpecification
{
	//Quad Shader
	std::shared_ptr<Shader> Shader;
	std::shared_ptr<RenderPass> RenderPass;
	bool BackfaceCulling = true;
	bool DepthTest = true;
	bool DepthWrite = true;
	bool Wireframe = false;
	float LineWidth = 1.0f;

	std::string DebugName;
};

class Pipeline
{
public:
	static std::shared_ptr<Pipeline> Create(const PipelineSpecification& spec);

	PipelineSpecification& GetSpecification();
	const PipelineSpecification& GetSpecification() const;

	//call after binding vertex buffer
	void Bind();
public:
	Pipeline(const PipelineSpecification& spec);
private:
	PipelineSpecification m_Specification;
	std::shared_ptr<VertexArray> m_VertexArray;
};
