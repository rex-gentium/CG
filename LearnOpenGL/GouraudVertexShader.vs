#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec4 objectColor;

float gouraudAmbient(float strength);
float gouraudDiffuse(float strength, vec3 surfaceNormal, vec3 lightDir);
float gouraudSpecular(float strength, vec3 viewDir, vec3 reflectionDir, float shininess);

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

	float ambientStrength = 0.2f;
	float diffuseStrength = 0.8f;
	float specularStrength = 1.0f;
	float shininess = 32.0f;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, norm);
	
    float ambient = gouraudAmbient(ambientStrength);
	float diffuse = gouraudDiffuse(diffuseStrength, norm, lightDir);
  	float specular = gouraudSpecular(specularStrength, viewDir, reflectDir, shininess);
    
    Color = vec4((ambient + diffuse + specular) * lightColor, 1.0f) * objectColor;
}

float gouraudAmbient(float strength) {
	return strength;
}

float gouraudDiffuse(float strength, vec3 surfaceNormal, vec3 lightDir) {
    return strength * max(dot(surfaceNormal, lightDir), 0.0);
}

float gouraudSpecular(float strength, vec3 viewDir, vec3 reflectionDir, float shininess) {
    return strength * pow(max(dot(viewDir, reflectionDir), 0.0), shininess);
}