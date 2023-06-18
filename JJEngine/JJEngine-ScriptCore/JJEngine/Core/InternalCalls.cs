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

        #region Entity

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
        internal static extern void TransformComponent_SetPosition(in UUIDType uuid, ref Vector3 toSet);

        #endregion


        /*#region Graphics

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Graphics_SetClearColor(float r, float g, float b);


        #endregion*/
    }
}
