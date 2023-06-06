﻿#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics.h"
#include "Core/Component/LightComponent.h"
#include <imgui.h>
void Graphics::AddRenderCommand(CommandType commandType, const VariableContainerType& variables)
{
	RenderCommand renderCommand;
	renderCommand.commandType = commandType;
	renderCommand.variables = variables;

	renderCommands.push_back(renderCommand);
}

void Graphics::ExecuteRenderCommands() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& command : renderCommands) {
		ExecutePreRenderCommand(command);

		ExecuteRenderCommand(command);
		ExecutePostRenderCommand(command);

	}
	renderCommands.clear();
}

void Graphics::ExecutePreRenderCommand(RenderCommand command)
{
	switch (command.commandType)
	{

	case CommandType::ForwardShaded:
	{
		/*
		* Z버퍼 FBO, 그림자 해상도, 폴리곤 오프셋, 그릴 대상, 사용할 셰이더, to world 행렬, to light 행렬, 빛 갯수
		*/

		ShadowSampling(command);

	}
	break;

	}
}

void Graphics::ExecuteRenderCommand(RenderCommand command)
{
	switch (command.commandType)
	{
	case CommandType::Forward:
	{
		auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);
		auto& ForwardShader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["ForwardShader"]);
		auto model = std::any_cast<Model&>(command.variables["Model"]);
		auto toWorld = std::any_cast<glm::mat4&> (command.variables["toWorld"]);
		auto toVP = std::any_cast<glm::mat4&> (command.variables["toVP"]);
		auto& samples = std::any_cast<std::vector<std::shared_ptr<FrameBuffer>>&>(command.variables["Shadow Maps"]);
		auto lights = std::any_cast<std::vector<LightInfo>&> (command.variables["Lights"]);
		std::vector<glm::vec3> lights_pos(lights.size());

		std::transform(lights.begin(), lights.end(), lights_pos.begin(), [](const LightInfo& light) {
			return light.toLightpos;
			});

		VAO->Bind();
		ForwardShader->Use();
		ForwardShader->SetInt("current_lights", lights.size());
		ForwardShader->SetMat4("toWorld", toWorld);
		ForwardShader->SetMat4("forNrm", glm::transpose(glm::inverse(toWorld)));
		ForwardShader->SetMat4("toVP", toVP);
		ForwardShader->SetFloatVector3("lights", lights_pos);

		for (const auto& mesh : model.GetMeshes())
		{
			mesh->GetMeshVBO()->SetData(sizeof(Vertex) * static_cast<size_t>(mesh->GetNumOfVertices()), &mesh->GetVertices()[0]);
			mesh->GetMeshVBO()->SetDataTypes({
				{ 0, DataType::Float3 }, //location=0, pos
				{ 1, DataType::Float3 }, //location=1, nrm
				{ 2, DataType::Float2 }, //location=2, uv
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
	case CommandType::ForwardShaded:
	{
		/*
		* Z버퍼 FBO, 그림자 해상도, 폴리곤 오프셋, 그릴 대상, 사용할 셰이더, to world 행렬, to light 행렬, 빛 갯수
		*/

		ForwardDrawWithShadow(command);

	}
	break;

	}
}

void Graphics::ExecutePostRenderCommand(RenderCommand command)
{
}

void Graphics::ShadowSampling(RenderCommand command)
{
	auto& shadowSamplingShader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["ShadowSamplingShader"]);
	auto& shadowMapFBO = std::any_cast<std::shared_ptr<FrameBuffer>&>(command.variables["ShadowMapFBO"]);
	const auto resolution = std::any_cast<glm::ivec2&>(command.variables["Shadow Resolution"]);
	const auto polygonOffset = std::any_cast<glm::ivec2&>(command.variables["Polygon Offset"]);
	auto model = std::any_cast<Model&>(command.variables["Model"]);
	auto toWorld = std::any_cast<glm::mat4&> (command.variables["toWorld"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command.variables["Lights"]);

	const int lightsNumber = lights.size();
	

	int bindNumber = 0;

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, resolution.x, resolution.y);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(polygonOffset.x, polygonOffset.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	ImGui::Begin("shadow");
	for (auto l : lights)
	{
		switch (l.type)
		{
		case LightType::Point:
		{
			for (auto vp : l.toLightVP)
			{
				shadowMapFBO->Bind(false);

				shadowSamplingShader->Use();
				shadowSamplingShader->SetMat4("toWorld", toWorld);
				shadowSamplingShader->SetMat4("toLight", vp);

				for (const auto& mesh : model.GetMeshes())
				{
					mesh->GetMeshVBO()->SetData(sizeof(Vertex) * static_cast<size_t>(mesh->GetNumOfVertices()), &mesh->GetVertices()[0]);
					mesh->GetMeshVBO()->SetDataTypes({
						{ 0, DataType::Float3 }, //location=0, pos
						{ 1, DataType::Float3 }, //location=1, nrm
						{ 2, DataType::Float2 }, //location=2, uv
						});
					if (mesh->GetNumOfIndices())
					{
						mesh->GetMeshEBO()->Bind();
						mesh->GetMeshEBO()->SetData(sizeof(int) * static_cast<size_t>(mesh->GetNumOfIndices()), &mesh->GetIndices()[0]);
					}
					mesh->GetMeshVBO()->BindToVertexArray();
					glDrawElements(GL_TRIANGLES, mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
				}

				std::shared_ptr<Texture> currentSample = Texture ::CopyTexture(shadowMapFBO->GetDepthTexture());
				
				const ImVec2 size{ 240,120 };
				currentSample->BindTexture(bindNumber);
				ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(currentSample->GetTextureID())), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		
				bindNumber++;
				auto find = std::find_if(shadows.begin(), shadows.end(), [=](shadowInfoByLight target) {return target.first == l.toLightpos; });
				if (find == shadows.end()) // not found
				{
					shadows.push_back(shadowInfoByLight{ l.toLightpos, std::tuple<std::vector<glm::mat4>,std::vector<std::shared_ptr<Texture>>>{} });
					get<0>(shadows.back().second).push_back(vp);
					get<1>(shadows.back().second).push_back(currentSample);
				}
				else // fouond
				{
					get<0>(find->second).push_back(vp);
					get<1>(find->second).push_back(currentSample);
				}
				shadowMapFBO->UnBind();

			}
		}
		break;
		}
	}
	ImGui::End();

	const std::tuple<int, int> viewportSize = Application::Instance().GetWindow()->GetWidthAndHeight();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, get<0>(viewportSize), get<1>(viewportSize));
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
}

void Graphics::ForwardDrawWithShadow(RenderCommand command)
{
	auto model = std::any_cast<Model&>(command.variables["Model"]);
	auto toWorld = std::any_cast<glm::mat4&> (command.variables["toWorld"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command.variables["Lights"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);
	auto& ForwardShader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["ForwardShader"]);
	auto toVP = std::any_cast<glm::mat4&> (command.variables["toVP"]);
	const int lightsNumber = lights.size();

	VAO->Bind();

	int i = 0;
	for (auto s : shadows)
	{
		const int sideNumber = get<0>(s.second).size();

		ForwardShader->Use();
		ForwardShader->SetInt("current_lights", lightsNumber);
		ForwardShader->SetInt("current_side", sideNumber);

		ForwardShader->SetMat4("toWorld", toWorld);
		ForwardShader->SetMat4("forNrm", glm::transpose(glm::inverse(toWorld)));
		ForwardShader->SetMat4("toVP", toVP);

		std::string posName = "sampleInfo[" + std::to_string(i) + "].pos";
		ForwardShader->SetFloat3(posName, s.first);


		std::string vpName = "sampleInfo[" + std::to_string(i) + "].vp[0]";
		ForwardShader->SetMatVector4(vpName, get<0>(s.second));

		const int sides = get<1>(s.second).size();
		for (int j = 0; j < sides; j++)
		{
			ForwardShader->SetInt("sampleInfo[" + std::to_string(i) + "].shadowMaps[" + std::to_string(j) + "]", get<1>(s.second)[j]->GetUnitID());
		}

		for (const auto& mesh : model.GetMeshes())
		{
			mesh->GetMeshVBO()->SetData(sizeof(Vertex) * static_cast<size_t>(mesh->GetNumOfVertices()), &mesh->GetVertices()[0]);
			mesh->GetMeshVBO()->SetDataTypes({
				{ 0, DataType::Float3 }, //location=0, pos
				{ 1, DataType::Float3 }, //location=1, nrm
				{ 2, DataType::Float2 }, //location=2, uv
				});
			if (mesh->GetNumOfIndices())
			{
				mesh->GetMeshEBO()->Bind();
				mesh->GetMeshEBO()->SetData(sizeof(int) * static_cast<size_t>(mesh->GetNumOfIndices()), &mesh->GetIndices()[0]);
			}
			mesh->GetMeshVBO()->BindToVertexArray();
			glDrawElements(GL_TRIANGLES, mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
		}
		i++;
	}
	for (auto s : shadows)
	{
		for (auto& t : get<1>(s.second))
		{
			t->UnBindTexture();
			//glBindTextureUnit(t, 0);
		}
	}
	shadows.clear();
	VAO->UnBind();
}
