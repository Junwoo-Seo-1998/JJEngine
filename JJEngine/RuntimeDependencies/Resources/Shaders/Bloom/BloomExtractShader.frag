in VS_OUT{ 
    vec2 UV;
} fs_in; 

layout (location = 0) out vec4 FragColor;

uniform sampler2D ToExtract;
uniform float Threshold;

void main()
{             
    vec3 color = texture(ToExtract, fs_in.UV).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > Threshold)
        FragColor = vec4(color, 1.0);
    else
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}  