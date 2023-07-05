#pragma vert

#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 toWorld;
uniform mat4 toLight;

void main()
{        
    gl_Position = toLight * toWorld * vec4(aPos, 1.0);
}

//---------------------------------------------------------------
#pragma frag

#version 460 core

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}