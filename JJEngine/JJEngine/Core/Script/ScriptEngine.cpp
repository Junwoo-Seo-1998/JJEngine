#include "ScriptEngine.h"
#include "Core/Type.h"
#include <iostream>
#include <fstream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/attrdefs.h>

#include "Core/Utils/Assert.h"
#include "Core/Utils/File.h"
#include "Core/Script/ScriptGlue.h"

#include <memory>

#include "Core/Scene.h"
#include "Core/Time.h"
#include "Core/Component/ScriptComponent.h"

namespace Script
{
	//todo: move this out to other class
	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },

		{ "System.Boolean", ScriptFieldType::Bool },

		{ "System.Char", ScriptFieldType::Char },

		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },

		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },


		{ "JJEngine.Vector2", ScriptFieldType::Vector2 },
		{ "JJEngine.Vector3", ScriptFieldType::Vector3 },
		{ "JJEngine.Vector4", ScriptFieldType::Vector4 },
		{ "JJEngine.Entity", ScriptFieldType::Entity },

	};

	ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
	{
		std::string typeName = mono_type_get_name(monoType);

		auto found = s_ScriptFieldTypeMap.find(typeName);
		if (found == s_ScriptFieldTypeMap.end())
			return ScriptFieldType::None;
		return found->second;
	}

	const char* ScriptFieldTypeToString(ScriptFieldType type)
	{
		switch (type)
		{
		case ScriptFieldType::Float: return "Float";
		case ScriptFieldType::Double: return "Double";

		case ScriptFieldType::Bool: return "Bool";

		case ScriptFieldType::Byte: return "Byte";
		case ScriptFieldType::Short: return "Short";
		case ScriptFieldType::Int: return "Int";
		case ScriptFieldType::Long: return "Long";

		case ScriptFieldType::UShort: return "UShort";
		case ScriptFieldType::UInt: return "UInt";
		case ScriptFieldType::ULong: return "ULong";

		case ScriptFieldType::Vector2: return "Vector2";
		case ScriptFieldType::Vector3: return "Vector3";
		case ScriptFieldType::Vector4: return "Vector4";

		case ScriptFieldType::Entity: return "Entity";
		}
		return "Undefined Type Name";
	}

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

	std::shared_ptr<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUIDType entityUUID)
	{
		auto found = s_Data->EntityInstances.find(entityUUID);
		if (found == s_Data->EntityInstances.end())
			return nullptr;
		
		return found->second;
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
			std::string className;

			if (strlen(nameSpace) != 0)
				className = std::format("{}.{}", nameSpace, name);
			else
				className = name;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);

			if(monoClass==entityClass)
				continue;

			bool is_subclass = mono_class_is_subclass_of(monoClass, entityClass, false);
			if(!is_subclass)
				continue;

			std::shared_ptr<ScriptClass> scriptClass = std::make_shared<ScriptClass>(nameSpace, name);
			s_Data->EntityClasses[className] = scriptClass;

			int fieldCount = mono_class_num_fields(monoClass);
			EngineLog::Trace("C# Class {} has {} fields: ", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);

				uint32_t flags= mono_field_get_flags(field);
				if(flags & MONO_FIELD_ATTR_PUBLIC)
				{
					MonoType* monoType = mono_field_get_type(field);
					ScriptFieldType type = MonoTypeToScriptFieldType(monoType);
					const char* typeName = ScriptFieldTypeToString(type);
					scriptClass->m_Fields[fieldName] = ScriptField{ type, fieldName, field };
					EngineLog::Trace("- Public ({}) {}", typeName, fieldName);
					
				}
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

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto found = fields.find(name);
		if (found == fields.end())
			return false;
		mono_field_get_value(m_Instance, found->second.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto found = fields.find(name);
		if (found == fields.end())
			return false;
		mono_field_set_value(m_Instance, found->second.ClassField, (void*)value);
		return true;
	}
}
