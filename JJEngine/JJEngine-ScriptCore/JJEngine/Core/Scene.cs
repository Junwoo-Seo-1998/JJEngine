namespace JJEngine
{
    public class Scene
    {
        public static Entity CreateEntity(string name = "UnNamed")
        {
            UUIDType uuid = new UUIDType();
            InternalCalls.Scene_CreateComponent(ref uuid, name);
            Entity entity = new Entity(uuid.LowBytes, uuid.HighBytes);
            return entity;
        }
    }
}
