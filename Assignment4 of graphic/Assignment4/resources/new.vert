
#version 120
varying vec4 color;
vec4 computeSpecular(in vec3 lightDir, in vec3 norm, in vec3 vertex_eye) {
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 r = reflect(-lightDir, norm);
    float VdotR = max(dot(normalize(-vertex_eye), r), 0.0);
    specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
    pow(VdotR, gl_FrontMaterial.shininess);
    return specular;
}
void main() {
    //gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; // or: ftransform()
    gl_Position = ftransform();
    vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
    vec3 vertex_eye = vec3(gl_ModelViewMatrix * gl_Vertex);


    vec3 lightDir = normalize(gl_LightSource[0].position.xyz - vertex_eye);
    vec3 lightDir1 = normalize(gl_LightSource[1].position.xyz - vertex_eye);

    //lightDir = normalize(vec3(gl_LightSource[0].position));

    float NdotL = max(dot(norm, lightDir), 0.0);
    float NdotL1 = max(dot(norm, lightDir1), 0.0);

    
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;
    diffuse = diffuse + gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse * NdotL1;
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    if (NdotL > 0){
        specular = computeSpecular(lightDir, norm, vertex_eye);
        specular = specular + computeSpecular(lightDir1,norm,vertex_eye);

    }

    gl_FrontColor = gl_Color*(diffuse + specular + ambient);
    //color = (diffuse + specular + ambient);

}