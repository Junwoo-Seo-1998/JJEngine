#include <algorithm>
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
	}

	for (const auto& command : renderCommands) {
		ExecuteRenderCommand(command);
	}

	for (const auto& command : renderCommands) {
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

		ShadowSampling(command);

	}
	break;

	}
}

void Graphics::ExecuteRenderCommand(RenderCommand command)
{
	switch (command.commandType)
	{

	case CommandType::ForwardShaded:
	{

		ForwardDrawWithShadow(command);
	}
	break;

	}
}

void Graphics::ExecutePostRenderCommand(RenderCommand command)
{

	for (auto& shadows : shadowsForModel)
	{
		for (auto& s : shadows)
		{
			for (auto& t : get<1>(s.second))
			{
				t->UnBindTexture();
				//glBindTextureUnit(t, 0);
			}
		}
		shadows.clear();
	}
	shadowsForModel.clear();

}

void Graphics::ShadowSampling(RenderCommand command)
{
	auto& shadowSamplingShader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["ShadowSamplingShader"]);
	auto& shadowMapFBO = std::any_cast<std::shared_ptr<FrameBuffer>&>(command.variables["ShadowMapFBO"]);
	const auto resolution = std::any_cast<glm::ivec2&>(command.variables["Shadow Resolution"]);
	const auto polygonOffset = std::any_cast<glm::ivec2&>(command.variables["Polygon Offset"]);
	auto modelInfo = std::any_cast<std::vector<ModelInfo>&>(command.variables["Model"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command.variables["Lights"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);

	const int lightsNumber = lights.size();
	
	int bindNumber = 0;

	ImGui::Begin("shadow");
	glEnable(GL_POLYGON_OFFSET_FILL);
	glViewport(0, 0, resolution.x, resolution.y);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glPolygonOffset(polygonOffset.x, polygonOffset.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	for (auto& info : modelInfo) // Draw for each model
	{
		for (const auto& mesh : info.model.GetMeshes()) // For all meshes in a target model
		{
			std::vector<shadowInfoByLight> shadows; // A container of shadows and light VP
			shadowSamplingShader->Use();
			shadowSamplingShader->SetMat4("toWorld", info.toWorld);
			for (const auto& l : lights) // For each light
			{
				switch (l.type)
				{
				case LightType::Point:
				{
					for (const auto& vp : l.toLightVP) // One light has 6 VP. Because it is point light.
					{
						glClear(GL_DEPTH_BUFFER_BIT);

						shadowMapFBO->Bind();
						shadowSamplingShader->SetMat4("toLight", vp);
						//Draw Call Start
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
						//Draw Call End

						//Ingui texture check function
						const ImVec2 size{ 240,120 };
						ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(shadowMapFBO->GetDepthTexture()->GetTextureID())), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						
						//Bind currently sampled FBO depth
						std::shared_ptr<Texture> currentSample = Texture::CopyTexture(shadowMapFBO->GetDepthTexture());
						currentSample->BindTexture(bindNumber);
						bindNumber++;


						//Find light. If it is, put vp and depth texture by the found light position.
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
			shadowsForModel.push_back(shadows);
		}
	}
	const std::tuple<int, int> viewportSize = Application::Instance().GetWindow()->GetWidthAndHeight();
	glViewport(0, 0, get<0>(viewportSize), get<1>(viewportSize));
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	ImGui::End();



}

void Graphics::ForwardDrawWithShadow(RenderCommand command)
{
	auto modelInfo = std::any_cast<std::vector<ModelInfo>&>(command.variables["Model"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command.variables["Lights"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command.variables["VAO"]);
	auto& ForwardShader = std::any_cast<std::shared_ptr<Shader>&>(command.variables["ForwardShader"]);
	auto toVP = std::any_cast<glm::mat4&> (command.variables["toVP"]);
	const int lightsNumber = lights.size();

	VAO->Bind();
	ForwardShader->Use();

	int shadowIndex = 0;
	for (auto& info : modelInfo)
	{
		for (const auto& mesh : info.model.GetMeshes())
		{
			std::vector<shadowInfoByLight> shadows = shadowsForModel[shadowIndex];
			int i = 0;
			for (auto s : shadows)
			{
				const int sideNumber = get<0>(s.second).size();

				ForwardShader->Use();
				ForwardShader->SetInt("current_lights", lightsNumber);
				ForwardShader->SetInt("current_side", sideNumber);

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

				ForwardShader->SetMat4("toWorld", info.toWorld);
				ForwardShader->SetMat4("forNrm", glm::transpose(glm::inverse(info.toWorld)));


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

				i++;
			}
			shadowIndex++;
		}
	}

	VAO->UnBind();
}
