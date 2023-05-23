layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;
layout (location = 2) in vec2 aUv;
uniform mat4 toWorld;
uniform mat4 toView;
uniform mat4 toLight;
uniform mat4 forNrm;
out vec3 vPos;
out vec3 vNrm;
out vec3 vToLighted;
void main()
{    
    vPos = vec3(toWorld * vec4(aPos, 1.0));
    gl_Position = toView * vec4(vPos, 1.0);
    vToLighted = vec3(toLight * vec4(vPos, 1.0));
    vNrm = vec3(forNrm * vec4(aNrm, 1.0));
}