layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNrm;
layout (location = 2) in vec2 aUv;

uniform mat4 toWorld;
uniform mat4 toLight;
out vec4 FragPos; 

void main()
{        
    
    FragPos = toWorld * vec4(aPos, 1.0);

    gl_Position = toLight * FragPos;
 

}