#pragma once
#include <memory>
#include <string>
#include <mono/metadata/image.h>
#include <mono/utils/mono-forward.h>

class ScriptEngine;
static std::shared_ptr<ScriptEngine> createInstance();
class ScriptEngine
{
public:
	static std::shared_ptr<ScriptEngine> instance();
	void Init();
	void InitCore();//testing
	void Shutdown();
private:
	void InitMono();
	void ShutdownMono();
private://inner helper functions
	std::shared_ptr<char[]> ReadBytes(const std::string& filepath, int& outSize);
	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
	void PrintAssemblyTypes(MonoAssembly* assembly);

	friend std::shared_ptr<ScriptEngine> createInstance();
	static std::shared_ptr<ScriptEngine> s_instance;
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
	}s_data;
};