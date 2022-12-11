#version 330

uniform sampler2D texture0;
in vec2 texCoord;

//Light Calculation variables
uniform vec3 diffuse_light = vec3(0.7, 0.7, 0.7);
uniform vec3 ambient = vec3(0.3);
in vec3 vNormal;
in vec3 vLight;

in vec4 color;
out vec4 LFragment;
uniform int mode = 0;
void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLight);

    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_light;
    LFragment = vec4((ambient + diffuse) * color.rgb, color.a);
    if (mode == 1)
        LFragment = LFragment * texture2D(texture0, texCoord.st);

}