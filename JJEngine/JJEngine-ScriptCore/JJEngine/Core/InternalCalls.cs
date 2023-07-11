using System;
using System.Runtime.CompilerServices;

namespace JJEngine
{
    internal static class InternalCalls
    {
        #region Debug

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Log(string str);

        #endregion

        #region Scene

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Scene_CreateEntity(ref UUIDType uuid, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Scene_GetEntityWithName(ref UUIDType uuid, string name);
        #endregion


        #region Entity

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_AddComponent(in UUIDType uuid, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent(in UUIDType uuid, Type componentType);

        #endregion

        #region Input

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsPressed_Key(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsPressed_Mouse(MouseCode mouseCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsTriggered_Key(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsTriggered_Mouse(MouseCode mouse);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsRepeating_Key(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsReleased_Key(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsReleased_Mouse(MouseCode mouseCode);

        #endregion

        #region TransformComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetPosition(in UUIDType uuid, out Vector3 returnParam);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetPosition(in UUIDType uuid, in Vector3 toSet);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetScale(in UUIDType uuid, out Vector3 returnParam);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetScale(in UUIDType uuid, in Vector3 toSet);

        #endregion

        #region ScriptComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void ScriptComponent_SetName(in UUIDType uuid, string name);

        #endregion


        #region RigidBody2DComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBody2DComponent_ApplyLinearImpulseToCenter(in UUIDType uuid,
            in Vector2 impulse, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void RigidBody2DComponent_ApplyLinearImpulse(in UUIDType uuid, in Vector2 impulse,
            in Vector2 point, bool wake);

        #endregion


        /*#region Graphics

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Graphics_SetClearColor(float r, float g, float b);


        #endregion*/

        //Scene change function

    }
}
