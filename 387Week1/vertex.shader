#version 330

uniform mat4 pMatrix = mat4(1.0); 
uniform mat4 mMatrix = mat4(1.0); 
uniform mat4 cMatrix = mat4(1.0);
uniform vec4 uColor = vec4(1.0f, 1.0f, 0.0f,1.0f);

in vec3 vertexPos;

//Light calculation data
uniform vec3 lightPos = vec3(0.0f, 10.0f, 2.0f);
in vec3 vertexNormal;
out vec3 vNormal;
out vec3 vLight;

out vec4 color;

uniform int mode = 0;
uniform mat4 instanceMatrix[1000];

void main() { 

	vec4 vPosition;

	if(mode == 1)
		vPosition = cMatrix * instanceMatrix[gl_InstanceID] * vec4(vertexPos.xyz, 1.0);
	else
		vPosition = cMatrix * mMatrix * vec4(vertexPos.xyz, 1.0);
	gl_Position = pMatrix * vPosition;
		
	


	//find normal in view space
	vNormal = mat3(cMatrix * mMatrix) * vertexNormal;
	//find view-space light vector
	vLight = lightPos - vPosition.xyz;
	
	color = uColor;
}
