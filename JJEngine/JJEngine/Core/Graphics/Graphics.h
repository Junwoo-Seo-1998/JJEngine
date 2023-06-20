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
#include "Core/Component/MaterialComponent.h"


using RenderCommandType = std::unordered_map<std::string, std::any>;

enum class LightType;
struct LightInfo
{
	LightType type;
	std::vector<glm::mat4> toLightVP;
	glm::vec3 toLightpos;
};


struct ModelInfo
{
	Model model;
	glm::mat4 toWorld;
	const MaterialComponent& material;
};

class Graphics
{
public:
	static std::shared_ptr<Graphics> GetInstance()
	{
		static std::shared_ptr<Graphics> instance(new Graphics());
		return instance;
	}

	void AddRenderCommand(const RenderCommandType& command);

	void ExecuteRenderCommands();

	void ExcuteInitializing();
	void ExecutePreRenderCommand(RenderCommandType& command);
	void ExecuteRenderCommand(RenderCommandType command);
	void ExecutePostRenderCommand(RenderCommandType command);

	void SetFinalFBOID(int ID) { finalFBO = ID; }

private:
	std::vector<RenderCommandType> renderCommands;
	//      for one light                 lightpos,       lightSideVP                      maps
	using shadowInfoByLight = std::pair<glm::vec3, std::tuple<std::vector<glm::mat4>, std::vector<std::shared_ptr<Texture>>>>;
	
	int bindNumber = 0;

	int finalFBO{0};
private:
	void ShadowSampling(RenderCommandType& command);
	void GBufferSampling(RenderCommandType& command);
	void ForwardDraw(RenderCommandType command);

	void DefferedDraw(RenderCommandType command);

};

