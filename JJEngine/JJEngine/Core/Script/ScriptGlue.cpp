#include "ScriptGlue.h"
#include "Core/Utils/Log.h"
#include "mono/jit/jit.h"
namespace Script
{
	#define ADD_INTERNAL_CALL(FuncName) mono_add_internal_call("JJEngine.InternalCalls::" #FuncName, FuncName)

	static void Debug_Log(MonoString* monoString)
	{
		char* to_print = mono_string_to_utf8(monoString);
		Log::Info(to_print);
		mono_free(to_print);
	}


	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Debug_Log);
	}
}
