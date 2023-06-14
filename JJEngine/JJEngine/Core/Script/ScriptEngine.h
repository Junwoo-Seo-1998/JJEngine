#pragma once
#include <memory>
#include <string>
#include <mono/metadata/image.h>
#include <mono/utils/mono-forward.h>
#include <filesystem>
namespace Script
{
	class ScriptClass;

	class ScriptEngine
	{
		friend ScriptClass;
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
	private:
		static void InitMono();
		static void ShutdownMono();
	private://inner helper functions
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);
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
}
