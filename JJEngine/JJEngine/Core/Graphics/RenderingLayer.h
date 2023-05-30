#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <any>
#include <glad.h>
#include "Mesh.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Core/Layer/Layer.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
enum class CommandType
{
	Transform,
	Shader,
	Texture,
	ShadowSampling,
	RenderTargetSampling,
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		renderCommands.clear();


	}

	void ExecuteRenderCommand(RenderCommand command)
	{
		switch (command.commandType)
		{
		case CommandType::ShadowSampling:
		{
			auto& FBOs = std::any_cast<std::vector<std::shared_ptr<FrameBuffer>>&>(command.variables["ShadowSaples"]);
			const auto& resolution = std::any_cast<glm::ivec2&>(command.variables["Shadow Resolution"]);
			const auto& polygonOffset = std::any_cast<glm::ivec2&>(command.variables["Polygon Offset"]);
			auto& model = std::any_cast<Model&>(command.variables["Model"]);
			auto& shader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["Shader"]);
			auto toWorld = std::any_cast<glm::mat4&> (command.variables["toWorld"]);
			auto& toLights = std::any_cast<std::vector<glm::mat4>&> (command.variables["toLights"]);


			for (int i = 0; i < 2; i++)
			{
				FBOs[i]->Bind();
				glBindFramebuffer(GL_FRAMEBUFFER, FBOs[i]->GetFrameBufferID());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, resolution.x, resolution.y);
				glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(polygonOffset.x, polygonOffset.y);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				shader->Use();
				shader->SetMat4("toWorld", toWorld);
				shader->SetMat4("toLight", toLights[i]);

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
					glDrawElements(GL_TRIANGLES, mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
				}
				FBOs[i]->UnBind();
				const std::tuple<int, int> viewportSize = Application::Instance().GetWindow()->GetWidthAndHeight();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, get<0>(viewportSize), get<1>(viewportSize));
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDisable(GL_CULL_FACE);
				glDisable(GL_POLYGON_OFFSET_FILL);
				glDrawBuffer(GL_BACK);
				glReadBuffer(GL_BACK);
			}
		}
		break;
		case CommandType::Draw:
		{			

			auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);
			auto& shader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["Shader"]);
			auto& model = std::any_cast<Model&>(command.variables["Model"]);

			VAO->Bind();
			shader->Use();
			auto toWorld = std::any_cast<glm::mat4&> (command.variables["toWorld"]);
			auto& toView = std::any_cast<glm::mat4&> (command.variables["toView"]);
			auto& lights = std::any_cast<std::vector<glm::vec3>&> (command.variables["lights"]);
			auto& samples = std::any_cast<std::vector<std::shared_ptr<FrameBuffer>>&>(command.variables["ShadowSaples"]);

			shader->SetMat4("toWorld", toWorld);
			shader->SetMat4("forNrm", glm::transpose(glm::inverse(toWorld)));
			shader->SetMat4("toView", toView);
			shader->SetFloatVector3("lights", lights);
			auto& toLights = std::any_cast<std::vector<glm::mat4>&> (command.variables["toLights"]);

			shader->SetMatVector4("toLight", toLights);



			samples[0]->GetDepthTexture()->BindTexture(100);
			samples[0]->GetDepthTexture()->BindTexture(101);
			shader->SetInt("shadowMaps[0]", 100);
			shader->SetInt("shadowMaps[1]", 101);


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
				glDrawElements(GL_TRIANGLES, mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
			}
			
			VAO->UnBind();

		}
		break;
		}
	}
private:
	std::vector<RenderCommand> renderCommands;
};