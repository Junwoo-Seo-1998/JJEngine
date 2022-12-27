#include "ScriptEngine.h"

#include <iostream>
#include <fstream>
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/threads.h"
static std::shared_ptr<ScriptEngine> createInstance()
{
	return std::make_shared<ScriptEngine>();
}

std::shared_ptr<ScriptEngine> ScriptEngine::s_instance= createInstance();
//ScriptEngine::ScriptEngineData ScriptEngine::s_data;

std::shared_ptr<ScriptEngine> ScriptEngine::instance()
{
	return s_instance;
}

void ScriptEngine::Init()
{
	InitMono();
}

void ScriptEngine::Shutdown()
{
	ShutdownMono();
}

void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");
	s_data.RootDomain = mono_jit_init("JJEngineJITRuntime");
	if(s_data.RootDomain == nullptr)
	{
		std::cerr << "mono jit init failed" << std::endl;
		throw;
	}
	s_data.AppDomain = mono_domain_create_appdomain("JJEngineAppDomain", nullptr);
	mono_thread_set_main(mono_thread_current());

	/*
	//for testing
	MonoAssembly* assembly = LoadCSharpAssembly("Resources/Scripts/JJEngine-ScriptCore.dll");
	PrintAssemblyTypes(assembly);
	MonoImage* image = mono_assembly_get_image(assembly);
	MonoClass* mainclass = mono_class_from_name(image, "JJEngine_ScriptCore", "main");
	if (mainclass == nullptr)
	{
		// Log error here
		std::cout << "error!";
	}
	// Allocate an instance of our class
	MonoObject* classInstance = mono_object_new(s_data.AppDomain, mainclass);

	// Call the parameterless (default) constructor
	mono_runtime_object_init(classInstance);
	*/
}

void ScriptEngine::ShutdownMono()
{
	mono_domain_set(mono_get_root_domain(), false);

	mono_domain_unload(s_data.AppDomain);
	s_data.AppDomain = nullptr;

	mono_jit_cleanup(s_data.RootDomain);
	s_data.RootDomain = nullptr;
}

std::shared_ptr<char[]> ScriptEngine::ReadBytes(const std::string& filepath, int& outSize)
{
	std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

	if (!stream)
	{
		// Failed to open the file
		return nullptr;
	}

	std::streampos end = stream.tellg();
	stream.seekg(0, std::ios::beg);
	int size = static_cast<int>(end - stream.tellg());

	if (size == 0)
	{
		// File is empty
		return nullptr;
	}

	std::shared_ptr<char[]> buffer(new char[size]);
	stream.read(buffer.get(), size);
	stream.close();

	outSize = size;
	return buffer;
}

MonoAssembly* ScriptEngine::LoadCSharpAssembly(const std::string& assemblyPath)
{
	int fileSize = 0;
	std::shared_ptr<char[]> fileData = ReadBytes(assemblyPath, fileSize);

	// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(fileData.get(), fileSize, 1, &status, 0);

	if (status != MONO_IMAGE_OK)
	{
		const char* errorMessage = mono_image_strerror(status);
		// Log some error message using the errorMessage data
		return nullptr;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
	mono_image_close(image);

	return assembly;
}

void ScriptEngine::PrintAssemblyTypes(MonoAssembly* assembly)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numTypes; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		printf("%s.%s\n", nameSpace, name);
	}
}


