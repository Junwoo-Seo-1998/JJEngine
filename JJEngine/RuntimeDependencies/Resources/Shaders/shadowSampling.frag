
void main()
{
    gl_FragDepth = gl_FragCoord.z / gl_FragCoord.w;
}