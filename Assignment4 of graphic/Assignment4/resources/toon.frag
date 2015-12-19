varying float intensity;
void main()
{
    vec4 color;
    
    // Hard code 4 possible light levels based on dot(N,L). Could pick any
    // number of levels, or pass in the number of levels and use a for-loop.
    if (intensity > 0.85)
        color = vec4(1.0,0.5,0.5,1.0);
    else if (intensity > 0.5)
        color = vec4(0.6,0.3,0.3,1.0);
    else if (intensity > 0.25)
        color = vec4(0.4,0.2,0.2,1.0);
    else
        color = vec4(0.2,0.1,0.1,1.0);
    gl_FragColor = color;
}