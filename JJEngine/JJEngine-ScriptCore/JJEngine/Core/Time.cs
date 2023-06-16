using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine
{
    class Time
    {
        public static float Delta
        {
            get;
            internal set;
        } = 0.016f;

        internal static void UpdateDelta(float delta)
        {
            Delta = delta;
        }
    }
}
