#pragma once
#include <memory>
#include <string>
#include <filesystem>
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
}

namespace Script
{
	class ScriptClass;
	class ScriptGlue;
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
		static std::unordered_map<std::string, std::shared_ptr<ScriptClass>> GetEntityClasses();
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
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		static void PrintAssemblyTypes(MonoAssembly* assembly);
		static MonoObject* InstantiateClass(MonoClass* monoClass);
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className);
		MonoObject* Instantiate();

		MonoMethod* GetMethod(const std::string& methodName, int paramCount);
		MonoObject* InvokeMethod(MonoObject* classInstance, MonoMethod* method, void** params);
	private:
		std::string m_NameSpace;
		std::string m_ClassName;
		MonoClass* m_MonoClass;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate();
	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};
}
