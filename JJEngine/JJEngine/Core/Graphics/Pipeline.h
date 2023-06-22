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

//������ Ư���� ����� ���� ���߿� Ȯ�强�� ���ؼ� Ŭ������ ����
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
