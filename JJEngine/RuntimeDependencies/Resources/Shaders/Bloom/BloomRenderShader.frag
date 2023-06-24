in VS_OUT{ 
    vec2 UV;
} fs_in; 

layout (location = 0) out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;


void main()
{             
    vec3 color = texture(scene, fs_in.UV).rgb;      
    vec3 bloomColor = texture(bloomBlur, fs_in.UV).rgb;
    
    FragColor = vec4(color+bloomColor, 1.0);
}