#pragma vert
#version 460 core

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

#pragma frag
#version 460 core

in VS_OUT{ 
    vec2 UV;
} fs_in; 

out vec4 FragColor;

uniform sampler2D FinalTexture;

void main()
{             
    FragColor = vec4(texture(FinalTexture, fs_in.UV).rgb, 1.0);
}  