#include "ScriptEngine.h"

#include <iostream>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

static std::shared_ptr<ScriptEngine> createInstance()
{
	return std::make_shared<ScriptEngine>();
}

std::shared_ptr<ScriptEngine> ScriptEngine::s_instance= createInstance();
ScriptEngine::ScriptEngineData ScriptEngine::s_data;

std::shared_ptr<ScriptEngine> ScriptEngine::instance()
{
	return s_instance;
}

void ScriptEngine::Init()
{
	mono_set_assemblies_path("mono/lib");
}

void ScriptEngine::Shutdown()
{
}


