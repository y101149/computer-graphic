varying vec3 norm;
varying vec3 vertex_eye;
varying vec4 color1;
uniform sampler2D diffuse_tex;
vec4 computeSpecular(in vec3 lightDir, in vec3 norm, in vec3 vertex_eye) {
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 r = reflect(-lightDir, norm);
    float VdotR = max(dot(normalize(-vertex_eye), r), 0.0);
    specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
    pow(VdotR, gl_FrontMaterial.shininess);
    return specular;
}
void main()
{
    vec3 n,lightDir,lightDir1;
    float NdotL, NdotL1;
    vec4 color;
    
    vec3 tex_color = texture2D(diffuse_tex, gl_TexCoord[0].st).rgb;
    vec3 pixel = tex_color * (gl_FrontMaterial.ambient*gl_LightSource[0].ambient + gl_FrontMaterial.ambient*gl_LightSource[1].ambient).rgb;
    
    /* a fragment shader can't write a verying variable, hence we need
     a new variable to store the normalized interpolated normal */
    n = normalize(norm);
    
    lightDir = normalize(gl_LightSource[0].position.xyz-vertex_eye);
    lightDir1 = normalize(gl_LightSource[1].position.xyz - vertex_eye);
//    
    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,lightDir),0.0);
    NdotL1 = max(dot(n,lightDir1),0.0);
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;
    diffuse = diffuse + gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse * NdotL1;
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    
    if (NdotL > 0.0) {
        pixel += tex_color * (gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse + gl_FrontMaterial.diffuse*gl_LightSource[1].diffuse).rgb * NdotL;
        specular = computeSpecular(lightDir, n, vertex_eye);
        specular = specular + computeSpecular(lightDir1,n, vertex_eye);
        
        pixel += vec3(specular);
        
    }
    color = vec4(pixel,1.0);
    //color = color1*(ambient+diffuse+specular);
    
    gl_FragColor = color;
}