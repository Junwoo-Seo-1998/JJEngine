#include "ScriptEngine.h"

#include <iostream>
#include <fstream>
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/threads.h"
#include "glad.h"
#include "Core/Utils/Assert.h"
#include "Core/Utils/File.h"

static std::string AppDomainName="JJ_AppDomain";

static std::shared_ptr<ScriptEngine> createInstance()
{
	return std::make_shared<ScriptEngine>();
}
void test_clearColor(float r, float g, float b)
{
	std::cout << "test_clearColor" << std::endl;
	glClearColor(r, g, b, 1.0f);
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

void ScriptEngine::InitCore()
{
	s_data.AppDomain = mono_domain_create_appdomain(AppDomainName.data(), nullptr);
	mono_domain_set(s_data.AppDomain, false);
#pragma region test
	mono_add_internal_call("JJEngine_ScriptCore.InternalCalls::Graphics_SetClearColor", (void*)test_clearColor);
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

	MonoClass* testclass = mono_class_from_name(image, "JJEngine_ScriptCore", "test");

	MonoObject* classInstance2 = mono_object_new(s_data.AppDomain, testclass);
	mono_runtime_object_init(classInstance2);

	MonoMethod* method = mono_class_get_method_from_name(testclass, "run", 1);
	MonoProperty* property = mono_class_get_property_from_name(testclass, "test_str");
	MonoString* mono_string = (MonoString*)mono_property_get_value(property, classInstance2, nullptr, nullptr);
	int len = mono_string_length(mono_string);
	std::cout << "len:" << std::to_string(len) << std::endl;
	char* csharp_str = mono_string_to_utf8(mono_string);
	std::string s = csharp_str;
	std::cout << s << std::endl;
	mono_free(csharp_str);



	MonoClassField* field = mono_class_get_field_from_name(testclass, "iamstr");
	MonoString* strval;
	mono_field_get_value(classInstance2, field, &strval);
	csharp_str = mono_string_to_utf8(strval);
	s = csharp_str;
	mono_free(csharp_str);

	//s = arr;
	std::cout << s << std::endl;



	float val = 20;
	void* params[1] = { &val };
	mono_runtime_invoke(method, classInstance2, params, nullptr);
#pragma endregion  
}

void ScriptEngine::Shutdown()
{
	ShutdownMono();
}



void ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");
	s_data.RootDomain = mono_jit_init("JJEngineJITRuntime");

	ENGINE_ASSERT(s_data.RootDomain != nullptr, "Mono JIT Init Failed");

	s_data.AppDomain = mono_domain_create_appdomain(AppDomainName.data(), nullptr);
	mono_domain_set(s_data.AppDomain, true);

	MonoAssembly* assembly = LoadCSharpAssembly("Resources/Scripts/JJEngine-ScriptCore.dll");
	PrintAssemblyTypes(assembly);

	MonoImage* image = mono_assembly_get_image(assembly);
	MonoClass* mono_class = mono_class_from_name(image, "JJEngine_ScriptCore", "main");
	MonoObject* object = mono_object_new(s_data.AppDomain, mono_class);
	mono_runtime_object_init(object);

/*#pragma region test
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

	MonoClass* testclass = mono_class_from_name(image, "JJEngine_ScriptCore", "test");

	MonoObject* classInstance2 = mono_object_new(s_data.AppDomain, testclass);
	mono_runtime_object_init(classInstance2);

	MonoMethod* method = mono_class_get_method_from_name(testclass, "run", 1);
	MonoProperty* property = mono_class_get_property_from_name(testclass, "test_str");
	MonoString* mono_string = (MonoString*)mono_property_get_value(property, classInstance2, nullptr, nullptr);
	int len = mono_string_length(mono_string);
	std::cout << "len:" << std::to_string(len) << std::endl;
	char* csharp_str = mono_string_to_utf8(mono_string);
	std::string s = csharp_str;
	std::cout << s << std::endl;
	mono_free(csharp_str);

	

	MonoClassField* field=mono_class_get_field_from_name(testclass, "iamstr");
	MonoString* strval;
	mono_field_get_value(classInstance2, field, &strval);
	csharp_str = mono_string_to_utf8(strval);
	s = csharp_str;
	mono_free(csharp_str);

	//s = arr;
	std::cout << s << std::endl;

	

	float val = 20;
	void* params[1] = { &val };
	mono_runtime_invoke(method, classInstance2, params, nullptr);
#pragma endregion  */
}

void ScriptEngine::ShutdownMono()
{
	mono_domain_set(mono_get_root_domain(), false);

	mono_domain_unload(s_data.AppDomain);
	s_data.AppDomain = nullptr;
	
	mono_jit_cleanup(s_data.RootDomain);
	s_data.RootDomain = nullptr;
}

MonoAssembly* ScriptEngine::LoadCSharpAssembly(const std::string& assemblyPath)
{
	auto [fileData, fileSize] = File::ReadFileToBytes(assemblyPath);

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


