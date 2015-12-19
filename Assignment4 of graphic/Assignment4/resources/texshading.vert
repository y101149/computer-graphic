
#version 120
varying vec3 norm;
varying vec3 vertex_eye;
varying vec4 color1;
void main() {
    //gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; // or: ftransform()
    gl_Position = ftransform();
    norm = normalize(gl_NormalMatrix * gl_Normal);
    vertex_eye = vec3(gl_ModelViewMatrix * gl_Vertex);
    color1 = gl_Color;
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
}