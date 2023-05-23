#define MAX_LIGHTS 2


in vec3 vPos;
in vec3 vNrm;
in vec3 vToLighted;
uniform vec3 cam;
uniform vec3 lights[MAX_LIGHTS];

out vec4 FragColor;

void main()
{
    vec3 finalColor;
    for(int i = 0; i < MAX_LIGHTS; i++)
    {
    vec3 lightDir = normalize(lights[i] - vPos);
    vec3 viewDir = normalize(cam - vPos);
    vec3 normal = normalize(vNrm);
    
    float ambientStrength = 0.2;
    float diffuseStrength = 0.8;
    float specularStrength = 0.5;
    float shininess = 32.0;
    
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diffuseStrength * max(dot(normal, lightDir), 0.0) * vec3(1.0, 1.0, 1.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * vec3(1.0, 1.0, 1.0);
    finalColor += ambient + diffuse + specular;
    }

    
    FragColor = vec4(finalColor, 1.0);
}