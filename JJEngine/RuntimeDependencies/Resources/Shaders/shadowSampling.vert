layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;
layout (location = 2) in vec2 aUv;

uniform mat4 toWorld;
uniform mat4 toLight;

void main()
{    
    gl_Position = toLight * toWorld * vec4(aPos, 1.0);
}