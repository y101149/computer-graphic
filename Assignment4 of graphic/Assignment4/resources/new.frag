varying vec4 color;
void main()
{
	//gl_FragColor = vec4(1.0,0.4,0.8,1.0);
    gl_FragColor = gl_Color;//using varying gl_color in default fragment shader
    //gl_FragColor = color;
}