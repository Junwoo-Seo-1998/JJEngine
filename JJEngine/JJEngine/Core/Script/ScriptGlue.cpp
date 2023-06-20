#include "ScriptGlue.h"
#include <iostream>
#include "ScriptEngine.h"
#include "box2d/b2_body.h"
#include "Core/Input/Input.h"
#include "Core/Utils/Log.h"
#include "Core/Utils/UUIDHelper.h"
#include "mono/jit/jit.h"

#include "Core/Component/Components.h"
#include "Core/Utils/DebugOnly.h"


namespace Script
{
	#define ADD_INTERNAL_CALL(FuncName) mono_add_internal_call("JJEngine.InternalCalls::" #FuncName, FuncName)

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	static void Debug_Log(MonoString* monoString)
	{
		char* to_print = mono_string_to_utf8(monoString);
		Log::Info(to_print);
		mono_free(to_print);
	}

	static bool Entity_HasComponent(UUIDType* uuid, MonoReflectionType* componentType)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);

		MonoType* monoComponentType = mono_reflection_type_get_type(componentType);
		ENGINE_ASSERT(s_EntityHasComponentFuncs.contains(monoComponentType), "This Component Type was not registered !!");
		return s_EntityHasComponentFuncs.at(monoComponentType)(entity);
	}

	static void TransformComponent_GetPosition(UUIDType* uuid, glm::vec3* returnParam)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);
		*returnParam = entity.Transform().Position;
	}

	static void TransformComponent_SetPosition(UUIDType* uuid, glm::vec3* toSet)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);
		entity.Transform().Position = *toSet;
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(UUIDType* uuid, glm::vec2* impulse, bool wake)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);

		auto& bodyComp = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)bodyComp.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2{ impulse->x, impulse->y }, wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulse(UUIDType* uuid, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = Script::ScriptEngine::GetSceneContext();
		ENGINE_ASSERT(scene);
		Entity entity = scene->GetEntity(*uuid);
		ENGINE_ASSERT(entity);

		auto& bodyComp = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)bodyComp.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2{ impulse->x, impulse->y }, b2Vec2{ point->x, point->y }, wake);
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

		ADD_INTERNAL_CALL(Entity_HasComponent);

		ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		ADD_INTERNAL_CALL(TransformComponent_SetPosition);

		ADD_INTERNAL_CALL(Input_IsPressed_Key);
		ADD_INTERNAL_CALL(Input_IsPressed_Mouse);

		ADD_INTERNAL_CALL(Input_IsTriggered_Key);
		ADD_INTERNAL_CALL(Input_IsTriggered_Mouse);

		ADD_INTERNAL_CALL(Input_IsRepeating_Key);

		ADD_INTERNAL_CALL(Input_IsReleased_Key);
		ADD_INTERNAL_CALL(Input_IsReleased_Mouse);

		ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
		ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
	}

	//helper
	template<typename ... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName{ typeid(Component).name() };

			//process of extracting out ComponentName
			//since it will look like "struct Namespace::ComponentName"
			size_t pos = typeName.find_last_of(':');
			if (pos == std::string::npos)//if it has no name space
			{
				//since it will look like "struct ComponentName"
				pos = typeName.find_last_of(' ');
			}

			std::string managedTypeName = std::format("JJEngine.{}", typeName.substr(pos + 1));

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());

			DEBUG_ONLY
			(
				if (managedType == nullptr)
				{
					EngineLog::Error("( Type : {} ) Is Not Existing in C#", managedTypeName);
					return;
				}
			);

			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(),
		//expand it with templates
		... );
	}

	template<typename ... Component>
	static void RegisterComponent(ComponentGroup<Component ...>)
	{
		RegisterComponent<Component ...>();
	}


	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}
}
