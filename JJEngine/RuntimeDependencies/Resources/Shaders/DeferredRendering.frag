#define MAX_LIGHTS 16
#define SIDES 6
uniform int current_side;
uniform int current_lights;


struct ShadowMapInfo
{
    mat4 vp[SIDES];
    vec3 pos;
    sampler2D shadowMaps[SIDES];
};

uniform ShadowMapInfo sampleInfo[MAX_LIGHTS];

float CalcShadow(int lightIndex, int sideIndex, vec3 currentPos)
{
    vec4 lighted = sampleInfo[lightIndex].vp[sideIndex] * vec4(currentPos, 1.f);
    vec3 shadowMapCoords = lighted.xyz / lighted.w;
    shadowMapCoords = shadowMapCoords * 0.5 + 0.5; 
    
    float shadow = 0.0;
    float bias = 0.0001; 
    
 
    float currentDepth = shadowMapCoords.z;
    float closestDepth = texture(sampleInfo[lightIndex].shadowMaps[sideIndex], shadowMapCoords.xy).r;
    
    shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if (lighted.z < 0) {
        shadow = 1.f;
    }
    return shadow;
}

in vec3 vPos;
uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAmb;
uniform sampler2D gDiff;
uniform sampler2D gSpec;
uniform vec3 cam;
out vec4 FragColor;


void main()
{
    vec3 uvPos = (vPos / 2.f) + 0.5f;
    vec2 UV = vec2(uvPos.x, uvPos.y);
    vec3 gPosition = texture(gPos, UV).rgb;
    vec3 gNormal = texture(gNormal, UV).rgb;  
    vec3 gAmbient = texture(gAmb, UV).rgb;
    vec3 gDiffuse = texture(gDiff, UV).rgb;
    vec3 gSpecular = texture(gSpec, UV).rgb;

    vec3 finalColor = {0.f, 0.f, 0.f};
    for(int i = 0; i < current_lights; i++)
    {
        vec3 lightDir = normalize(sampleInfo[i].pos  - gPosition);
        vec3 viewDir = normalize(gPosition - cam);
        vec3 normal = normalize(gNormal);
        
        float ambientStrength = 0.1;
        float diffuseStrength = 0.6;
        float specularStrength = 0.4;
        float shininess = 32.0;
        
        vec3 ambient = gAmbient * ambientStrength * vec3(1.0, 1.0, 1.0);
        vec3 diffuse = gDiffuse * diffuseStrength * max(dot(normal, lightDir), 0.0) * vec3(1.0, 1.0, 1.0);
        
        float shadow = 1.f;

        for(int j = 0; j < current_side; j++)
        {
            shadow = min(shadow, CalcShadow(i, j, gPosition));
        }
        
        vec3 reflectDir = reflect(lightDir, normal);
        vec3 specular = gSpecular * specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * vec3(1.0, 1.0, 1.0);
        
        finalColor += ambient + diffuse * (1.0 - shadow) + specular * (1.0 - shadow);
    }

    
    FragColor = vec4(finalColor, 1.0);
}