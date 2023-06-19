#include "ScriptEngine.h"
#include "Core/Type.h"
#include <iostream>
#include <fstream>
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/threads.h"
#include "glad.h"
#include "Core/Utils/Assert.h"
#include "Core/Utils/File.h"
#include "Core/Script/ScriptGlue.h"

#include <memory>

#include "Core/Scene.h"
#include "Core/Time.h"
#include "Core/Component/ScriptComponent.h"

namespace Script
{
	struct ScriptEngineData;
	static std::unique_ptr<ScriptEngineData> s_Data = nullptr;

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		MonoMethod* UpdateDelta = nullptr;
		ScriptClass EntityClass;

		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> EntityClasses;
		std::unordered_map<UUIDType, std::shared_ptr<ScriptInstance>> EntityInstances;

		//runtime stuff
		Scene* SceneContext = nullptr;
	};

	static std::string AppDomainName = "JJ_AppDomain";

	
	void ScriptEngine::Init()
	{
		s_Data = std::make_unique<ScriptEngineData>();
		InitMono();
		LoadAssembly("Resources/Scripts/JJEngine-ScriptCore.dll");
		LoadAppAssembly("Resources/Scripts/GameScript.dll");

		LoadAssemblyClasses();
		//PrintAssemblyTypes(s_Data->CoreAssembly);
		for(auto p:s_Data->EntityClasses)
		{
			EngineLog::Trace("Subclass of Entity : {}", p.first);
		}


		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		ScriptClass TimeClass{ "JJEngine", "Time", true };
		s_Data->UpdateDelta = TimeClass.GetMethod("UpdateDelta", 1);
		s_Data->EntityClass = ScriptClass{ "JJEngine", "Entity", true };

		//MonoObject* instance = s_Data->EntityClass.Instantiate();
		/*MonoMethod* update = s_Data->EntityClass.GetMethod("OnUpdate", 0);
		s_Data->EntityClass.InvokeMethod(instance, update, nullptr);*/
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
	}

	void ScriptEngine::UpdateTime()
	{
		float time = Time::GetDelta();
		void* param = &time;
		mono_runtime_invoke(s_Data->UpdateDelta, nullptr, &param, nullptr);
	}

	void ScriptEngine::StartRuntime(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::StopRuntime()
	{
		s_Data->SceneContext = nullptr;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain(AppDomainName.data(), nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadMonoAssembly(filepath.string());
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		//debug
		//PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = LoadMonoAssembly(filepath.string());
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		//debug
		//PrintAssemblyTypes(s_Data->AppAssembly);
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& script = entity.GetComponent<ScriptComponent>();
		if (Script::ScriptEngine::EntityClassExists(script.Name))
		{
			std::shared_ptr<ScriptInstance> instance = std::make_shared<ScriptInstance>(s_Data->EntityClasses[script.Name], entity);
			s_Data->EntityInstances[entity.GetUUID()] = instance;

			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity)
	{
		const UUIDType entityUUID = entity.GetUUID();
		ENGINE_ASSERT(s_Data->EntityInstances.contains(entityUUID), "Was Not Instantiate!!");
		std::shared_ptr<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnUpdate();
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	std::unordered_map<std::string, std::shared_ptr<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName)
	{
		return s_Data->EntityClasses.contains(fullName);
	}


	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");
		s_Data->RootDomain = mono_jit_init("JJEngineJITRuntime");
		ENGINE_ASSERT(s_Data->RootDomain != nullptr, "Mono JIT Init Failed");
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), true);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	MonoAssembly* ScriptEngine::LoadMonoAssembly(const std::filesystem::path& assemblyPath)
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
		mono_image_close(image);

		return assembly;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "JJEngine", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = std::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);

			if(monoClass==entityClass)
				continue;

			bool is_subclass = mono_class_is_subclass_of(monoClass, entityClass, false);
			if(is_subclass)
			{
				s_Data->EntityClasses[fullName] = std::make_shared<ScriptClass>(nameSpace, name);
			}
		}
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

			EngineLog::Trace("{}.{}\n", nameSpace, name);
		}
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& className, bool isCore)
		:m_NameSpace(nameSpace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, nameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int paramCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), paramCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* classInstance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, classInstance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 2);

		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		if(!m_OnCreateMethod)
			m_OnCreateMethod = s_Data->EntityClass.GetMethod("OnCreate", 0);

		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 0);
		if (!m_OnUpdateMethod)
			m_OnUpdateMethod = s_Data->EntityClass.GetMethod("OnUpdate", 0);

		{//call entity ctor
			auto uuid = entity.GetUUID().as_bytes();
			void* params[2] =
			{
				(void*)&uuid[0],
				(void*)&uuid[8]
			};
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, params);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod, nullptr);
	}

	void ScriptInstance::InvokeOnUpdate()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, nullptr);
	}
}
