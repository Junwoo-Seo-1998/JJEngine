#include "Core/test.h"
#include "Core/Engine.h"
#include "Core/SceneManager.h"
#include "Core/Graphics/Shader.h"
#include "TestScene.h"
#include "Core/ComponentManager.h"
int main(void)
{
	JJEngine::init();
	TestScene test1{" test 1 "};
	TestScene test2{" test 2 "};
	auto shader = Shader::CreateShaderFromFile({ 
		{ ShaderType::VertexShader,{"common.glsl","vertex.vert"}},
		{ ShaderType::FragmentShader,{"vertex.frag"} }
	});
	JJEngine::GetSceneManager()->enrollScene(0, test1);
	JJEngine::GetSceneManager()->enrollScene(1, test2);
	JJEngine::GetSceneManager()->setNextScene(1);
	JJEngine::update();
	JJEngine::shutdown();
	hello();
	return 0;
}