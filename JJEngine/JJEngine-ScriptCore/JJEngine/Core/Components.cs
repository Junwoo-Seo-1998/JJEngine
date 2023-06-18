using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }

    }

    public class TransformComponent : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(Entity.UUID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(Entity.UUID, ref value);
            }
        }
    }
}
