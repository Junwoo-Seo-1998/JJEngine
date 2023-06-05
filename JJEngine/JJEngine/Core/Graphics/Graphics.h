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

	void ExecuteRenderCommand(RenderCommand command);
private:
	std::vector<RenderCommand> renderCommands;
};