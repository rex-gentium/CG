#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec4 objectColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

float phongAmbient(float strength);
float phongDiffuse(float strength, vec3 surfaceNormal, vec3 lightDir);
float phongSpecular(float strength, vec3 viewDir, vec3 reflectionDir, float shininess);

void main()
{	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	
    float ambient = phongAmbient(ambientStrength);
	float diffuse = phongDiffuse(diffuseStrength, norm, lightDir);
  	float specular = phongSpecular(specularStrength, normalize(viewPos - FragPos), reflect(-lightDir, norm), 32.0f);
    
    color = vec4((ambient + diffuse + specular) * lightColor, 1.0f) * objectColor;
}

float phongAmbient(float strength) {
	return strength;
}

float phongDiffuse(float strength, vec3 surfaceNormal, vec3 lightDir) {
    return strength * max(dot(surfaceNormal, lightDir), 0.0);
}

float phongSpecular(float strength, vec3 viewDir, vec3 reflectionDir, float shininess) {
    return strength * pow(max(dot(viewDir, reflectionDir), 0.0), shininess);
}