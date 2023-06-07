in vec4 FragPos; 

void main()
{ 
    float depthValue = FragPos.z / FragPos.w; 
    gl_FragDepth = depthValue;
}