
testing 

including test first

#include "./Resources/Shaders/TestPreProcessorinclude.glsl"

including test second

#include "./Resources/Shaders/TestPreProcessorinclude.glsl"

including test end
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

#pragma vert

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 ViewProjection;
uniform mat4 Transform;
out vec2 TexCoord;

#define anything

asd

#define   super
#define sss  


#     ifdef anything
ifdef testing
//this is test 
#endif
 # ifndef anything
ifndef testing
#endif


//this is test 

void main() /*this is test*/
{
    /*this is test22*/
    gl_Position = ViewProjection * Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aUv;
}vertvert

#pragma frag

void main() /*this is test*/
{
    /*this is test22*/
    gl_Position = ViewProjection * Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aUv;
}fragfrag

