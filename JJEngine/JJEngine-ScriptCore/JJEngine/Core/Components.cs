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
                InternalCalls.TransformComponent_GetPosition(in Entity.UUID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(in Entity.UUID, in value);
            }
        }
    }

    public class RigidBody2DComponent : Component   
    {

        
        public void ApplyLinearImpulse(in Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulseToCenter(in Entity.UUID, in impulse, wake);
        }
        public void ApplyLinearImpulse(in Vector2 impulse, in Vector2 worldPoint, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulse(in Entity.UUID, in impulse, in worldPoint, wake);
        }
    }
}
