#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics.h"
#include "Core/Component/LightComponent.h"
#include <imgui.h>

void Graphics::AddRenderCommand(const RenderCommandType& command)
{
	renderCommands.push_back(command);
}

void Graphics::ExecuteRenderCommands() {
	
	ExcuteInitializing();



	for (auto& command : renderCommands) {
		ExecutePreRenderCommand(command);
	}
	//.
		ExecuteRenderCommand(renderCommands[0]);


	for (const auto& command : renderCommands) {
		ExecutePostRenderCommand(command);
	}

	renderCommands.clear();
}

void Graphics::ExcuteInitializing()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	bindNumber = 0;

}

void Graphics::ExecutePreRenderCommand(RenderCommandType& command)
{
	
	ShadowSampling(command);
	GBufferSampling(command);
	
}

void Graphics::ExecuteRenderCommand(RenderCommandType command)
{
	glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
	DefferedDraw(command);
}

void Graphics::ExecutePostRenderCommand(RenderCommandType command)
{
	auto& GBufferFBO = std::any_cast<std::shared_ptr<FrameBuffer>&>(command["GBufferFBO"]);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBufferFBO->GetHandle());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);
	const int width = GBufferFBO->GetSpecification().Width;
	const int height = GBufferFBO->GetSpecification().Height;
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	ForwardDraw(command);		
	
}

void Graphics::ShadowSampling(RenderCommandType& command)
{
	auto& shadowMapFBO = std::any_cast<std::shared_ptr<FrameBuffer>&>(command["ShadowMapFBO"]);
	const auto resolution = std::any_cast<glm::ivec2&>(command["Shadow Resolution"]);
	const auto polygonOffset = std::any_cast<glm::ivec2&>(command["Polygon Offset"]);
	auto modelInfos = std::any_cast<std::vector<ModelInfo>&>(command["Models"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command["VAO"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command["Lights"]);
	const int lightsNumber = lights.size();

	std::vector<shadowInfoByLight> shadows; // A container of shadows and light VP

	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glPolygonOffset(polygonOffset.x, polygonOffset.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glViewport(0, 0, resolution.x, resolution.y);

	for (const auto& l : lights) // For each light
	{
		switch (l.type)
		{
		case LightType::PointLight:
		{
			for (const auto& vp : l.toLightVP) // One light has 6 VP. Because it is point light.
			{

				shadowMapFBO->Bind();
				glClear(GL_DEPTH_BUFFER_BIT);

				//Draw Call Start
				for (auto& modelInfo : modelInfos)
				{
					if (!modelInfo.material.isShadowed) continue;
					auto& shadowSamplingShader = modelInfo.material.shadowSamplingShader;
					shadowSamplingShader->Use();
					shadowSamplingShader->SetMat4("toLight", vp);
					shadowSamplingShader->SetMat4("toWorld", modelInfo.toWorld);

					for (const auto& mesh : modelInfo.model.GetMeshes()) // For all meshes in a target model
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
				}
					
					
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
	command["ShadowMaps"] = shadows;
	

	const std::tuple<int, int> viewportSize = Application::Instance().GetWindow()->GetWidthAndHeight();
	glViewport(0, 0, get<0>(viewportSize), get<1>(viewportSize));
	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glClear(GL_DEPTH_BUFFER_BIT);



}

void Graphics::GBufferSampling(RenderCommandType& command)
{

	auto modelInfos = std::any_cast<std::vector<ModelInfo>&>(command["Models"]);
	auto toVP = std::any_cast<glm::mat4&> (command["toVP"]);
	auto& GBufferFBO = std::any_cast<std::shared_ptr<FrameBuffer>&>(command["GBufferFBO"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command["Lights"]);
	const int lightsNumber = lights.size();


	GBufferFBO->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (auto& modelInfo : modelInfos)
	{
		if (modelInfo.material.type == MaterialType::Forward) continue;
		auto& GBufferShader = modelInfo.material.defferedFirstPassShader;
		GBufferShader->Use();
		GBufferShader->SetMat4("toWorld", modelInfo.toWorld);
		GBufferShader->SetMat4("toVP", toVP);
		GBufferShader->SetMat4("toWorld", modelInfo.toWorld);
		GBufferShader->SetMat4("forNrm", glm::transpose(glm::inverse(modelInfo.toWorld)));


		for (const auto& mesh : modelInfo.model.GetMeshes())
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
	}
	
	const int GBufferSize = GBufferFBO->GetSpecification().Formats.Formats.size() - 1;
	for (int i = 0; i < GBufferSize; i++)
	{
		GBufferFBO->GetColorTexture(i)->BindTexture(bindNumber);
		bindNumber++;
	}

	GBufferFBO->UnBind();




}

void Graphics::ForwardDraw(RenderCommandType command)
{
	auto modelInfos = std::any_cast<std::vector<ModelInfo>&>(command["Models"]);
	auto lights = std::any_cast<std::vector<LightInfo>&> (command["Lights"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command["VAO"]);
	auto toVP = std::any_cast<glm::mat4&> (command["toVP"]);
	auto camPos = std::any_cast<glm::vec3&> (command["camPos"]);

	const int lightsNumber = lights.size();

	VAO->Bind();


	const std::vector<shadowInfoByLight> shadows = std::any_cast<std::vector<shadowInfoByLight>&>(command["ShadowMaps"]);


	int lightIndex = 0;

	for (auto& modelInfo : modelInfos)
	{
		if (modelInfo.material.type == MaterialType::Deferred) continue;
		auto& ForwardShader = modelInfo.material.forwardShader;
		ForwardShader->Use();
		ForwardShader->SetInt("current_lights", lightsNumber);
		ForwardShader->SetFloat3("cam", camPos);
		ForwardShader->SetMat4("toVP", toVP);
		ForwardShader->SetMat4("toWorld", modelInfo.toWorld);
		ForwardShader->SetMat4("toWorld", modelInfo.toWorld);
		ForwardShader->SetMat4("forNrm", glm::transpose(glm::inverse(modelInfo.toWorld)));
		
		for (const auto& s : shadows)
		{
			const int sideNumber = get<0>(s.second).size();
			ForwardShader->SetInt("current_side", sideNumber);
			std::string posName = "sampleInfo[" + std::to_string(lightIndex) + "].pos";

			ForwardShader->SetFloat3(posName, s.first);

			std::string vpName = "sampleInfo[" + std::to_string(lightIndex) + "].vp[0]";
			ForwardShader->SetMatVector4(vpName, get<0>(s.second));

			const int sides = get<1>(s.second).size();
			for (int j = 0; j < sides; j++)
			{
				ForwardShader->SetInt("sampleInfo[" + std::to_string(lightIndex) + "].shadowMaps[" + std::to_string(j) + "]", get<1>(s.second)[j]->GetUnitID());
			}
			
			lightIndex++;
		}


		for (const auto& mesh : modelInfo.model.GetMeshes())
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
	}

}


void Graphics::DefferedDraw(RenderCommandType command)
{
	if (command.find("DefferedShader") == command.end()) return;
	auto& DefferedSecondPassShader = std::any_cast<std::shared_ptr<Shader>&>(command["DefferedShader"]);

	auto lights = std::any_cast<std::vector<LightInfo>&> (command["Lights"]);
	auto& VAO = std::any_cast<std::shared_ptr<VertexArray>&>(command["VAO"]);
	auto camPos = std::any_cast<glm::vec3&> (command["camPos"]);
	auto FSQ = std::any_cast<Mesh&> (command["FSQ"]);
	const int lightsNumber = lights.size();
	auto& GBuffer = std::any_cast<std::shared_ptr<FrameBuffer>&>(command["GBufferFBO"]);
	DefferedSecondPassShader->Use();
	DefferedSecondPassShader->SetInt("current_lights", lightsNumber);
	DefferedSecondPassShader->SetInt("gPos", GBuffer->GetColorTexture(0)->GetUnitID());
	DefferedSecondPassShader->SetInt("gNormal", GBuffer->GetColorTexture(1)->GetUnitID());
	DefferedSecondPassShader->SetInt("gAmb", GBuffer->GetColorTexture(2)->GetUnitID());
	DefferedSecondPassShader->SetInt("gDiff", GBuffer->GetColorTexture(3)->GetUnitID());
	DefferedSecondPassShader->SetInt("gSpec", GBuffer->GetColorTexture(4)->GetUnitID());

	DefferedSecondPassShader->SetFloat3("cam", camPos);

	const std::vector<shadowInfoByLight> shadows = std::any_cast<std::vector<shadowInfoByLight>&>(command["ShadowMaps"]);

	int lightIndex = 0;
	for (const auto& s : shadows)
	{
		const int sideNumber = get<0>(s.second).size();
		DefferedSecondPassShader->SetInt("current_side", sideNumber);

		std::string posName = "sampleInfo[" + std::to_string(lightIndex) + "].pos";
		DefferedSecondPassShader->SetFloat3(posName, s.first);

		std::string vpName = "sampleInfo[" + std::to_string(lightIndex) + "].vp[0]";
		DefferedSecondPassShader->SetMatVector4(vpName, get<0>(s.second));

		const int sides = get<1>(s.second).size();
		for (int j = 0; j < sides; j++)
		{
			DefferedSecondPassShader->SetInt("sampleInfo[" + std::to_string(lightIndex) + "].shadowMaps[" + std::to_string(j) + "]", get<1>(s.second)[j]->GetUnitID());
		}
		lightIndex++;
	}


	FSQ.GetMeshVBO()->SetData(sizeof(Vertex) * static_cast<size_t>(FSQ.GetNumOfVertices()), &FSQ.GetVertices()[0]);
	FSQ.GetMeshVBO()->SetDataTypes({
		{ 0, DataType::Float3 }, //location=0, pos
		{ 1, DataType::Float3 }, //location=1, nrm
		{ 2, DataType::Float2 }, //location=2, uv
		});
	if (FSQ.GetNumOfIndices())
	{	   
		FSQ.GetMeshEBO()->Bind();
		FSQ.GetMeshEBO()->SetData(sizeof(int) * static_cast<size_t>(FSQ.GetNumOfIndices()), &FSQ.GetIndices()[0]);
	}
	FSQ.GetMeshVBO()->BindToVertexArray();
	glDrawElements(GL_TRIANGLES, FSQ.GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);

	


}

