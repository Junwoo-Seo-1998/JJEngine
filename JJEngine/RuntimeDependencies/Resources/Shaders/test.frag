#define MAX_LIGHTS 2


in vec3 vPos;
in vec3 vNrm;
in vec4 vToLighted[MAX_LIGHTS];
uniform vec3 cam;
uniform vec3 lights[MAX_LIGHTS];
uniform sampler2D shadowMaps[MAX_LIGHTS];
out vec4 FragColor;

float CalcShadow(vec4 fragPosLightSpace, int lightIndex)
{
    // 그림자 맵에서 광원에 대한 텍스처 좌표를 계산
    vec3 shadowMapCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    shadowMapCoords = shadowMapCoords * 0.5 + 0.5; // 좌표를 [0, 1] 범위로 매핑
    
    float shadow = 0.0;
    float bias = 0.001; // 그림자 맵의 정확성을 위한 바이어스 조절
    
    // 그림자 맵에서 광원에 대한 깊이 값을 가져와 그림자 여부를 계산
    float currentDepth = shadowMapCoords.z;
    float closestDepth = texture(shadowMaps[lightIndex], shadowMapCoords.xy).r;
    
    shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}

void main()
{
    vec3 finalColor = {0.f, 0.f, 0.f};
     for(int i = 0; i < 2; i++)
    {
        vec3 lightDir = normalize(lights[i] - vPos);
        vec3 viewDir = normalize(cam - vPos);
        vec3 normal = normalize(vNrm);
        
        float ambientStrength = 0.1;
        float diffuseStrength = 0.6;
        float specularStrength = 0.4;
        float shininess = 32.0;
        
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        vec3 diffuse = diffuseStrength * max(dot(normal, lightDir), 0.0) * vec3(1.0, 1.0, 1.0);
        float shadow = CalcShadow(vToLighted[i], i);
        
        vec3 reflectDir = reflect(lightDir, normal);
        vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * vec3(1.0, 1.0, 1.0);
        
        finalColor += ambient + diffuse * (1.0 - shadow) + specular * (1.0 - shadow);
    }

    
    FragColor = vec4(finalColor, 1.0);
}