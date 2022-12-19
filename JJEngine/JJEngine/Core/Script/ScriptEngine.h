#pragma once
#include <memory>
#include <mono/utils/mono-forward.h>
class ScriptEngine;
static std::shared_ptr<ScriptEngine> createInstance();
class ScriptEngine
{
public:
	static std::shared_ptr<ScriptEngine> instance();
	static void Init();

	static void Shutdown();
private:
	static void InitMono();
	static void ShutdownMono();

	friend std::shared_ptr<ScriptEngine> createInstance();
	static std::shared_ptr<ScriptEngine> s_instance;
	static struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
	}s_data;
};