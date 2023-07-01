#define MAX_LIGHTS 16
#define ZERO 0.000000001

//Interface
in VS_OUT{ 
	vec3 FragPos;
	vec3 NormalVector;
    vec2 UV;
} fs_in; 

out vec4 FragColor;

//Types
struct MaterialTexture
{
    sampler2D Diffuse;
    sampler2D Specular;
    sampler2D Emissive;
    float Shininess;
};

struct MaterialData
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
    vec3 Emissive;
};

struct LightData
{
    int LightType;
    mat4 ViewProjection;
    sampler2D ShadowMap;
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float InnerAngle;
    float OuterAngle;
    float FallOff;
};

struct AttenuationData
{
    float c1;
    float c2;
    float c3;
};

struct FogData
{
    float Near;
    float Far;
    vec3 Color;
};

//Uniform
uniform MaterialTexture MatTexture;
uniform vec3 CameraPosition;
uniform int LightNumbers;
uniform LightData Light[MAX_LIGHTS];
uniform vec3 globalAmbient;
uniform AttenuationData Attenuation;
uniform FogData Fog;

//Support Functions
vec3 ComputeReflection(vec3 normalVector, vec3 lightVector)
{
    return 2*(dot(normalVector,lightVector))*normalVector-lightVector;
}

float ComputeAttenuation(in AttenuationData attenuation, float lightDistance)
{
    return min(1.f/(attenuation.c1+attenuation.c2*lightDistance+attenuation.c3*pow(lightDistance,2)),1.f);
}

float ComputeSpotLightEffect(in LightData light, vec3 lightVector)
{
    vec3 spotLightVector=normalize(light.Direction);
    vec3 fromLightToVertex=-lightVector;

    float cos_Angle=dot(spotLightVector,fromLightToVertex);
    float cos_Outer=cos(light.OuterAngle);
    float cos_Inner=cos(light.InnerAngle);
    if(cos_Angle<cos_Outer)
        return 0.f;//no effect
    if(cos_Angle>cos_Inner)
        return 1.f;//max effect
    
    return pow((cos_Angle-cos_Outer)/(cos_Inner-cos_Outer), light.FallOff);
}

//Light Functions
vec3 ComputePointLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),ZERO), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    return att*(ambient+diffuse+specular);
}

vec3 ComputeDirectionLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=normalize(-light.Direction);

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),ZERO), material.Shininess);
    return ambient+diffuse+specular;
}

vec3 ComputeSpotLight(in LightData light, in AttenuationData attenuation, in MaterialData material, vec3 fragPos, vec3 normalVector, vec3 viewVector)
{
    vec3 lightVector=light.Position-fragPos;
    float lightDistance=length(lightVector);
	lightVector=lightVector/lightDistance;//normalize(lightVector)

	vec3 reflection=ComputeReflection(normalVector,lightVector);
	//
    vec3 ambient = material.Ambient*light.Ambient;
	vec3 diffuse = material.Diffuse*light.Diffuse*max(dot(normalVector,lightVector),0.f);
	vec3 specular = material.Specular*light.Specular*pow(max(dot(reflection,viewVector),ZERO), material.Shininess);

    float att=ComputeAttenuation(attenuation,lightDistance);
    float spotLightEffect=ComputeSpotLightEffect(light, lightVector);
    return att*(ambient+spotLightEffect*(diffuse+specular));
}

float CalcShadow(in LightData light, vec3 currentPos)
{
    vec4 lighted = light.ViewProjection * vec4(currentPos, 1.f);
    vec3 shadowMapCoords = lighted.xyz / lighted.w;
    
    float shadow = 0.0;
    
    if(shadowMapCoords.z < 1.f)
    {    
        vec3 lightVector=light.Position-currentPos;

        shadowMapCoords = (shadowMapCoords + 1.f) / 2.f; 
        float currentDepth = shadowMapCoords.z;
        float bias =  0.0005f;

        float closestDepth = texture(light.ShadowMap, shadowMapCoords.xy).r;
                shadow = currentDepth > (closestDepth + bias) ? 1.0 : 0.0;
    }
    return shadow;
}

//Fog Function
vec3 ComputeFog(in FogData fog,vec3 color, float viewDistance)
{
    float s=max(fog.Far-viewDistance,0.0001f)/max(fog.Far-fog.Near,0.0000001f);
    return s*color+(1-s)*fog.Color;
}

//main
void main()
{
    vec3 NormalVector=normalize(fs_in.NormalVector);
    vec3 ViewVector=CameraPosition-fs_in.FragPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;

    MaterialData Material;
    Material.Diffuse=texture(MatTexture.Diffuse, fs_in.UV).rgb; 
    Material.Ambient=Material.Diffuse*0.001;
    Material.Specular=texture(MatTexture.Specular, fs_in.UV).rgb;
    Material.Shininess=MatTexture.Shininess;
    Material.Emissive=texture(MatTexture.Emissive, fs_in.UV).rgb;

    vec3 TotalColor=Material.Emissive;
    
    float shadow = 1.f;
    for(int i=0; i<LightNumbers; ++i)
    {
        switch (Light[i].LightType)
        {
            case 0:
                TotalColor+=ComputePointLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, Material, fs_in.FragPos, NormalVector, ViewVector);
                break;
        }

        shadow = min(shadow, CalcShadow(Light[i], fs_in.FragPos));
        TotalColor *= (1-shadow);
    }
    


    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);
	FragColor = vec4(globalAmbient+TotalColor, 1.0);
}