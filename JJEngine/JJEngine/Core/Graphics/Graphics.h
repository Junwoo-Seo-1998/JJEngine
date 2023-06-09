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


using VariableContainerType = std::unordered_map<std::string, std::any>;


enum class CommandType
{
	Forward = 1,
	Deffered,
	ForwardShaded,
	DefferedShaded
};


struct RenderCommand
{
	CommandType commandType = CommandType::Forward;
	VariableContainerType variables;
};

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
};

class Graphics
{
public:
	static std::shared_ptr<Graphics> GetInstance()
	{
		static std::shared_ptr<Graphics> instance(new Graphics());
		return instance;
	}

	void AddRenderCommand(CommandType commandType, const VariableContainerType& variables);

	void ExecuteRenderCommands();

	void ExecutePreRenderCommand(RenderCommand command);
	void ExecuteRenderCommand(RenderCommand command);
	void ExecutePostRenderCommand(RenderCommand command);

private:
	std::vector<RenderCommand> renderCommands;
	//                                 라이트 위치마다                 6개의 light VP         6개의 매핑된 그림자 텍스쳐
	using shadowInfoByLight = std::pair<glm::vec3, std::tuple<std::vector<glm::mat4>, std::vector<std::shared_ptr<Texture>>>>;
	std::vector<std::vector<shadowInfoByLight>> shadowsForModel;

	int shadowIndex = 0;
private:
	void ShadowSampling(RenderCommand command);
	void ForwardDrawWithShadow(RenderCommand command);
};