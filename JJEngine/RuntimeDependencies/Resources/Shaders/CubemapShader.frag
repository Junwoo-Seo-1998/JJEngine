out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube environmentMap;
void main()
{
    FragColor = texture(environmentMap, TexCoords);
}