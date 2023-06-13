#define MAX_LIGHTS 16

uniform int current_lights;
uniform vec3 lightPos[MAX_LIGHTS];

in vec3 vPos;
uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D gAmb;
uniform sampler2D gDiff;
uniform sampler2D gSpec;
uniform sampler2D gShad;
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
    vec3 gShadowValue = texture(gShad, UV).rgb;

    vec3 finalColor = {0.f, 0.f, 0.f};
    for(int i = 0; i < current_lights; i++)
    {
        vec3 lightDir = normalize(lightPos[i] - gPosition);
        vec3 viewDir = normalize(gPosition - cam);
        vec3 normal = normalize(gNormal);
        
        float ambientStrength = 0.1;
        float diffuseStrength = 0.6;
        float specularStrength = 0.4;
        float shininess = 32.0;
        
        vec3 ambient = gAmbient * ambientStrength * vec3(1.0, 1.0, 1.0);
        vec3 diffuse = gDiffuse * diffuseStrength * max(dot(normal, lightDir), 0.0) * vec3(1.0, 1.0, 1.0);
        
        vec3 reflectDir = reflect(lightDir, normal);
        vec3 specular = gSpecular * specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * vec3(1.0, 1.0, 1.0);
        
        finalColor += ambient + (diffuse + specular) * (1 - gShadowValue.x);
    }

    
    FragColor = vec4(finalColor, 1.0);
}