layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 ViewProjection;
uniform mat4 Transform;
out vec2 TexCoord;

void main()
{
    gl_Position = ViewProjection * Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aUv;
}