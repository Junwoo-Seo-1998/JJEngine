layout (location = 0) in vec3 pos; 

out vec3 vPos;

void main(void) 
{
    vPos = pos;
    gl_Position = vec4(pos, 1.f);
}