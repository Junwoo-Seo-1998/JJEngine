#pragma once
#include <memory>
#include <string>
#include <filesystem>
#include <map>
#include <unordered_map>

#include "Core/Entity/Entity.hpp"

class Scene;

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoString MonoString;
	typedef struct _MonoType MonoType;
}

namespace Script
{
	class ScriptClass;
	class ScriptGlue;
	class ScriptInstance;
	class ScriptEngine
	{
		friend ScriptClass;
		friend ScriptGlue;
	public:
		static void Init();
		static void Shutdown();
		static void UpdateTime();

		static void StartRuntime(Scene* scene);
		static void StopRuntime();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static std::unordered_map<std::string, std::shared_ptr<ScriptClass>> GetEntityClasses();
		static std::shared_ptr<ScriptInstance> GetEntityScriptInstance(UUIDType entityUUID);

		static bool EntityClassExists(const std::string& fullName);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity);

		static Scene* GetSceneContext();
		static MonoImage* GetCoreAssemblyImage();

	private:
		static void InitMono();
		static void ShutdownMono();

		
	private://inner helper functions
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);
		static void LoadAssemblyClasses();

		static void PrintAssemblyTypes(MonoAssembly* assembly);
		static MonoObject* InstantiateClass(MonoClass* monoClass);
	};

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool,
		Char,
		Byte, Short, Int, Long,
		UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity,
	};

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};

	class ScriptClass
	{
		friend ScriptEngine;
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className, bool isCore = false);
		MonoObject* Instantiate();

		MonoMethod* GetMethod(const std::string& methodName, int paramCount);
		MonoObject* InvokeMethod(MonoObject* classInstance, MonoMethod* method, void** params);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_NameSpace;
		std::string m_ClassName;
		MonoClass* m_MonoClass;

		std::map<std::string, ScriptField> m_Fields;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate();

		std::shared_ptr<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();
			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:

		std::shared_ptr<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static char s_FieldValueBuffer[8];
	};
}
