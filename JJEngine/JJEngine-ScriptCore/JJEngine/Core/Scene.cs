namespace JJEngine
{
    public class Scene
    {
        public static Entity CreateEntity(string name = "UnNamed")
        {
            UUIDType uuid = new UUIDType();
            InternalCalls.Scene_CreateEntity(ref uuid, name);
            Entity entity = new Entity(uuid.LowBytes, uuid.HighBytes);
            return entity;
        }

        public static Entity GetEntity(string name)
        {
            UUIDType uuid = new UUIDType();
            if (!InternalCalls.Scene_GetEntityWithName(ref uuid, name))
            {
                return null;
            }

            return new Entity(uuid.LowBytes, uuid.HighBytes);
        }
    }
}
