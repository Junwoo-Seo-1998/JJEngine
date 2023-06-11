layout (location = 0) out int FragEntityID;
uniform int EntityID;

void main()
{
    FragEntityID=EntityID;
} 