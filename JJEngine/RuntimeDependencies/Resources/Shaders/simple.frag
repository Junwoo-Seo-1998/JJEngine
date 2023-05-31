out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D TextureUnit;
uniform vec4 Color;
void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor=Color*texture(TextureUnit, TexCoord);
} 