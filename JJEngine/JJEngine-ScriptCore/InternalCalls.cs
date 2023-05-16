using System;
using System.Runtime.CompilerServices;

namespace JJEngine_ScriptCore
{
    internal static class InternalCalls
    {
        #region Graphics

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Graphics_SetClearColor(float r, float g, float b);


        #endregion
    }
}
