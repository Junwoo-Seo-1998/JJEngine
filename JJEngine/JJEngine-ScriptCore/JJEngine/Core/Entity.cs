using System;

namespace JJEngine
{
    public class Entity
    {
        public readonly UUIDType UUID;
        protected Entity()
        {
            UUID = new UUIDType(0, 0);
        }
        internal Entity(UInt64 low, UInt64 high)
        {
            UUID = new UUIDType(low, high);
        }
        
        public Vector3 Position
        {
            get
            {
                InternalCalls.Entity_GetPosition(in UUID, out Vector3 position);
                return position;
            }
            set
            {   
                InternalCalls.Entity_SetPosition(in UUID, ref value);
            }
        }

        protected virtual void OnCreate() { }
        protected virtual void OnUpdate() { }
    }
}
