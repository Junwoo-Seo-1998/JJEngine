#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include <Core/SceneManager.h>
#include "EditorLayer.h"
#include "TestScene.h"
#include "Core/Layer/LayerManager.h"
#include "Core/Utils/Log.h"
#include "Core/Utils/Math/MatrixMath.h"
#include "Core/Utils/ShaderPreprocessor.h"
#include "Core/Utils/File.h"

class EditorApp : public Application
{
	virtual void UserDefinedInit(std::string GameData) override
	{
		Application::UserDefinedInit(GameData);
		/*auto test1 = std::make_shared<TestScene>("Test");
		GetSceneManager()->enrollScene(0, test1);
		GetSceneManager()->setNextScene(0);*/
		EngineLog::Info(glm::vec2{ 1.f });
		EngineLog::Info(glm::vec3{ 1.f });
		EngineLog::Info(glm::vec4{1.f});
		EngineLog::Info(glm::mat3{ 1.f });
		EngineLog::Info(MatrixMath::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 1.f, 0.1f, 1000.f));
		GetLayerManager()->PushLayer(std::make_shared<EditorLayer>());

		//ShaderPreprocesssor::Preprocess("Resources/Shaders/TestPreProcessor.glsl");

		//std::unordered_map<ShaderType, std::string> temp = ShaderPreprocesssor::Preprocess("ShaderPreprocessUnitTest/PragmaTest.glsl");
		//ASSERT(temp.size() == 5, "#pragma testing - Not enough shader types");
		//ASSERT(temp[ShaderType::None] == File::ReadFileToString("ShaderPreprocessUnitTest/PragmaTestNone.glsl"), "#pragma testing - None type failed");
		//ASSERT(temp[ShaderType::VertexShader] == File::ReadFileToString("ShaderPreprocessUnitTest/PragmaTestVert.glsl"), "#pragma testing - VertexShader type failed");
		//ASSERT(temp[ShaderType::FragmentShader] == File::ReadFileToString("ShaderPreprocessUnitTest/PragmaTestFrag.glsl"), "#pragma testing - FragmentShader type failed");
		//ASSERT(temp[ShaderType::GeometryShader] == File::ReadFileToString("ShaderPreprocessUnitTest/PragmaTestGeom.glsl"), "#pragma testing - GeometryShader type failed");
		//ASSERT(temp[ShaderType::TessellationShader] == File::ReadFileToString("ShaderPreprocessUnitTest/PragmaTestTess.glsl"), "#pragma testing - TessellationShader type failed");

		//temp = ShaderPreprocesssor::Preprocess("ShaderPreprocessUnitTest/Define_Ifdef_Test.glsl");
		//ASSERT(temp.size() == 1, "#ifdef testing - Not correct shader type number");
		//ASSERT(temp[ShaderType::None] == File::ReadFileToString("ShaderPreprocessUnitTest/Define_Ifdef_Test_answer.glsl"), "#ifdef testing failed");

		//temp = ShaderPreprocesssor::Preprocess("ShaderPreprocessUnitTest/Include_Test.glsl");
		//ASSERT(temp.size() == 5, "#include testing - Not correct shader type number");
		//ASSERT(temp[ShaderType::None] == File::ReadFileToString("ShaderPreprocessUnitTest/Include_Test_answer.glsl"), "#include testing failed");
	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<EditorApp>();
}
