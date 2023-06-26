in VS_OUT{ 
    vec2 UV;
} fs_in; 

out vec4 FragColor;

uniform sampler2D HDRTexture;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(HDRTexture, fs_in.UV).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}  