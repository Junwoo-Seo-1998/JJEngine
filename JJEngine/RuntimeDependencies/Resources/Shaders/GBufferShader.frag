in VS_OUT{ 
	vec3 FragPos;
	vec3 NormalVector;
    vec2 UV;
} fs_in; 

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gDiffuse;
layout (location = 3) out vec4 gSpecular;
layout (location = 4) out vec4 gEmissive;

struct MaterialTexture
{
    sampler2D Diffuse;
    sampler2D Specular;
    sampler2D Emissive;
    float Shininess;
};

uniform MaterialTexture MatTexture;

void main()
{
    gPosition = vec4(fs_in.FragPos, 1.f);

    gNormal = vec4(normalize(fs_in.NormalVector), 1.f);

    gDiffuse = vec4(texture(MatTexture.Diffuse, fs_in.UV).rgb, 1.f);

    gSpecular = vec4(texture(MatTexture.Specular, fs_in.UV).rgb, MatTexture.Shininess);

    gEmissive = texture(MatTexture.Emissive, fs_in.UV);
}