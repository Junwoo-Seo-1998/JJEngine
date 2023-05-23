#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <any>
#include <glad.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Layer/Layer.h"
enum class CommandType
{
	Transform,
	Shader,
	Texture,
	Draw,
};

using VariableContainerType = std::unordered_map<std::string, std::any>;

struct RenderCommand
{
	CommandType commandType = CommandType::Transform;
	VariableContainerType variables;
};

class RenderingLayer : public Layer
{
public:
	void OnAttach() override
	{
		RenderingLayer::GetInstance();
	}

	void OnUpdate() override
	{
		RenderingLayer::GetInstance()->ExecuteRenderCommands();
	}

	static std::shared_ptr<RenderingLayer> GetInstance()
	{
		static std::shared_ptr<RenderingLayer> instance(new RenderingLayer());
		return instance;
	}

	void AddRenderCommand(CommandType commandType, const VariableContainerType& variables)
	{
		RenderCommand renderCommand;
		renderCommand.commandType = commandType;
		renderCommand.variables = variables;

		renderCommands.push_back(renderCommand);
	}

	void ExecuteRenderCommands() {
		for (const auto& command : renderCommands) {
			ExecuteRenderCommand(command);
		}
	}

	void ExecuteRenderCommand(RenderCommand command)
	{
		switch (command.commandType)
		{
		case CommandType::Draw:
		{			
			auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);
			auto& shader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["Shader"]);
			VAO->Bind();
			shader->Use();
			auto& model = std::any_cast<Model&>(command.variables["Model"]);
			for (const auto& mesh : model.GetMeshes())
			{
				mesh->GetMeshVBO()->SetData(sizeof(Vertex) * static_cast<size_t>(mesh->GetNumOfVertices()), &mesh->GetVertices()[0]);
				mesh->GetMeshVBO()->SetDataTypes({
				{0, DataType::Float3}, //location=0, pos
				{1, DataType::Float3}, //location=1, nrm
				{2, DataType::Float2}, //location=2, uv
					});
				if (mesh->GetNumOfIndices())
				{
					mesh->GetMeshEBO()->Bind();
					mesh->GetMeshEBO()->SetData(sizeof(int) * static_cast<size_t>(mesh->GetNumOfIndices()), &mesh->GetIndices()[0]);
				}
				mesh->GetMeshVBO()->BindToVertexArray();
				glDrawElements(GL_LINES, mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);

			}
			
			VAO->UnBind();
		}
		break;
		}
	}
private:
	std::vector<RenderCommand> renderCommands;
};