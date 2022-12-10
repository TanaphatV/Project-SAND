#version 330

//Light Calculation variables
uniform vec3 diffuse_light = vec3(0.7, 0.7, 0.7);
uniform vec3 ambient = vec3(0.3);
in vec3 vNormal;
in vec3 vLight;

in vec4 color; 
out vec4 LFragment; 

void main() { 
   // vec3 N = normalize(vNormal);
   // vec3 L = normalize(vLight);
 
    vec3 diffuse = max(dot(vNormal, vLight), 0.0) * diffuse_light;
    LFragment = vec4((ambient + diffuse ) * color.rgb, color.a);
    
}
