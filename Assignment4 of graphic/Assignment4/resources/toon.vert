varying float intensity;
void main()
{
    
     /* first transform the normal into eye space and normalize the result */
     vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

     // Normalize direction to light
    vec3 vertex_eye = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 lightDir = normalize(vec3(gl_LightSource[0].position.xyz - vertex_eye));
    // Send intensity to fragment shader in varying variable
    intensity = max(dot(normal, lightDir), 0.0);
    gl_Position = ftransform();
}