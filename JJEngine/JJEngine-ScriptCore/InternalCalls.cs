using System;
using System.Runtime.CompilerServices;

namespace JJEngine_ScriptCore
{
    internal static class InternalCalls
    {
        #region Debug

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Log(string str);

        #endregion


        /*#region Graphics

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Graphics_SetClearColor(float r, float g, float b);


        #endregion*/
    }
}
