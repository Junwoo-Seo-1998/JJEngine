in VS_OUT{ 
    vec2 UV;
} fs_in; 

#define MAX_LIGHTS 16
#define ZERO 0.000000001

struct LightData
{
    int LightType;
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

struct MaterialData
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
    vec3 Emissive;
};

struct FogData
{
    float Near;
    float Far;
    vec3 Color;
};

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

vec3 ComputeFog(in FogData fog,vec3 color, float viewDistance)
{
    float s=max(fog.Far-viewDistance,0.0001f)/max(fog.Far-fog.Near,0.0000001f);
    return s*color+(1-s)*fog.Color;
}

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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gEmissive;

uniform vec3 CameraPosition;

uniform int LightNumbers;
uniform LightData Light[MAX_LIGHTS];


uniform vec3 globalAmbient;
uniform AttenuationData Attenuation;

uniform FogData Fog;

out vec4 FragColor;
void main()
{
    

    vec3 FragPos=texture(gPosition, fs_in.UV).rgb;
    vec3 NormalVector=normalize(texture(gNormal, fs_in.UV).rgb);
    
    vec3 ViewVector=CameraPosition-FragPos;
    float ViewDistance=length(ViewVector);
    ViewVector=ViewVector/ViewDistance;

    MaterialData Material;
    Material.Diffuse=texture(gDiffuse, fs_in.UV).rgb;
    Material.Ambient=Material.Diffuse*0.001;
    vec4 specular=texture(gSpecular, fs_in.UV);
    Material.Specular=specular.rgb;
    Material.Shininess=specular.a;
    Material.Emissive=texture(gEmissive, fs_in.UV).rgb;

    vec3 TotalColor=Material.Emissive;
    for(int i=0; i<LightNumbers; ++i)
    {
        switch (Light[i].LightType)
        {
            case 0:
                TotalColor+=ComputePointLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
        }
    }
    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);
	FragColor = vec4(globalAmbient+TotalColor, 1.0);
}