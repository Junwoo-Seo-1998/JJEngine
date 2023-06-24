layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
out VS_OUT{ 
    vec2 UV;
} vs_out; 

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vs_out.UV = aUV;
}