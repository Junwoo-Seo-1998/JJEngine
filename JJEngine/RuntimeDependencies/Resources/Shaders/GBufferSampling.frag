//Basic Properties
in vec3 vNrm;
in vec3 vPos;
in vec2 vUV;

layout (location = 0) out vec4 outPosition;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAmbient;
layout (location = 3) out vec4 outDiffuse;
layout (location = 4) out vec4 outSpecular;

void main()
{    
    vec4 ambient = vec4(1.f);
    vec4 diffuse = vec4(1.f);
    vec4 specular = vec4(1.f);  
    outPosition = vec4(vPos, 1.f);
    outNormal = vec4(normalize(vNrm), 1.f);
    outAmbient = ambient;
    outDiffuse = diffuse;
    outSpecular = specular;
}