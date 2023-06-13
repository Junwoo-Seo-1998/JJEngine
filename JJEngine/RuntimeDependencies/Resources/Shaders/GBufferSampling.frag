

//Basic Properties
in vec3 vNrm;
in vec3 vPos;
in vec2 vUV;

layout (location = 0) out vec4 outPosition;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAmbient;
layout (location = 3) out vec4 outDiffuse;
layout (location = 4) out vec4 outSpecular;
layout (location = 5) out vec4 outShadowValue;



//Shadow Properties
#define MAX_LIGHTS 16
#define SIDES 6

uniform int current_side;
uniform int current_lights;

struct ShadowMapInfo
{
    mat4 vp[SIDES];
    sampler2D shadowMaps[SIDES];
};
uniform ShadowMapInfo sampleInfo[MAX_LIGHTS];


float CalcShadow(int lightIndex, int sideIndex)
{
    vec4 lighted = sampleInfo[lightIndex].vp[sideIndex] * vec4(vPos, 1.f);
    vec3 shadowMapCoords = lighted.xyz / lighted.w;
    shadowMapCoords = shadowMapCoords * 0.5 + 0.5; 
    
    float shadow = 0.0;
    float bias = 0.001; 
    
 
    float currentDepth = shadowMapCoords.z;
    float closestDepth = texture(sampleInfo[lightIndex].shadowMaps[sideIndex], shadowMapCoords.xy).r;
    
    shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    if (lighted.z < 0) {
        shadow = 1.f;
    }
    return shadow;
}





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

    float shadow = 1.f;
    for(int i = 0; i < current_lights; i++)
    {  
        for(int j = 0; j < current_side; j++)
        {
            shadow = min(shadow, CalcShadow(i, j));
        }
    }

    outShadowValue = vec4(shadow);
}