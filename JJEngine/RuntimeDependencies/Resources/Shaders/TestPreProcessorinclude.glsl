#ifndef includetest
#define includetest
testing 

#include "./Resources/Shaders/simple.vert"
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
uniform mat4 ViewProjection;
uniform mat4 Transform;
out vec2 TexCoord;

#define thisistest

#     ifdef anything
ifdef testing
//this is test 
#endif
 # ifndef anything
ifndef testing
#endif


#endif