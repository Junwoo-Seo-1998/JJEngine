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

        public T AddComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
                return GetComponent<T>();

            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(in UUID, componentType);
            T component = new T() { Entity = this };
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(in UUID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };
            return component;
        }

        protected virtual void OnCreate()
        {

        }
        protected virtual void OnUpdate()
        {

        }
        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(in UUID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(in UUID, in value);
            }
        }

    }
}
