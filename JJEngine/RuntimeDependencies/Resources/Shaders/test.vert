layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;
layout (location = 2) in vec2 aUv;

uniform int MAX_LIGHT;


uniform mat4 toWorld;
uniform mat4 toVP;
uniform mat4 toLight[MAX_LIGHT];
uniform mat4 forNrm;
out vec3 vPos;
out vec3 vNrm;
out vec4 vToLighted[MAX_LIGHT];
void main()
{    
    vPos = vec3(toWorld * vec4(aPos, 1.0));
    gl_Position = toVP * vec4(vPos, 1.0);
    for (int i = 0; i < MAX_LIGHT; ++i)
    {
        vToLighted[i] = toLight[i] * vec4(vPos, 1.0);
    }
    vNrm = vec3(forNrm * vec4(aNrm, 1.0));
}