#include "ScriptGlue.h"

#include <iostream>

#include "ScriptEngine.h"
#include "Core/Input/Input.h"
#include "Core/Utils/Log.h"
#include "Core/Utils/UUIDHelper.h"
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

	static void Entity_GetPosition(UUIDType* uuid, glm::vec3* returnParam)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);
		*returnParam = entity.Transform().Position;
	}

	static void Entity_SetPosition(UUIDType* uuid, glm::vec3* toSet)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);
		entity.Transform().Position = *toSet;
	}

	static bool Input_IsPressed_Key(KeyCode key)
	{
		return Input::IsPressed(key);
	}

	static bool Input_IsPressed_Mouse(MouseCode mouse)
	{
		return Input::IsPressed(mouse);
	}

	static bool Input_IsTriggered_Key(KeyCode key)
	{
		return Input::IsTriggered(key);
	}

	static bool Input_IsTriggered_Mouse(MouseCode mouse)
	{
		return Input::IsTriggered(mouse);
	}

	static bool Input_IsRepeating_Key(KeyCode key)
	{
		return Input::IsRepeating(key);
	}

	static bool Input_IsReleased_Key(KeyCode key)
	{
		return Input::IsReleased(key);
	}

	static bool Input_IsReleased_Mouse(MouseCode mouse)
	{
		return Input::IsReleased(mouse);
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Debug_Log);

		ADD_INTERNAL_CALL(Entity_GetPosition);
		ADD_INTERNAL_CALL(Entity_SetPosition);

		ADD_INTERNAL_CALL(Input_IsPressed_Key);
		ADD_INTERNAL_CALL(Input_IsPressed_Mouse);

		ADD_INTERNAL_CALL(Input_IsTriggered_Key);
		ADD_INTERNAL_CALL(Input_IsTriggered_Mouse);

		ADD_INTERNAL_CALL(Input_IsRepeating_Key);

		ADD_INTERNAL_CALL(Input_IsReleased_Key);
		ADD_INTERNAL_CALL(Input_IsReleased_Mouse);
	}
}
