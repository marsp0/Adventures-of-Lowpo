#version 330 core

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;


void main()
{
	// ambient
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;
	// diffuse 
	vec3 normal = normalize(normal);
	vec3 lightDir = normalize(lightPosition - fragPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// // specular
	// float specularStrength = 0.4;
	// vec3 viewDir = normalize(viewPosition - fragPosition);
	// vec3 reflectDir = reflect(-lightDir, normal);  
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	// vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (diffuse + ambient) * vec3(.749,.839,.2);
	// vec3 result = normal;
	FragColor = vec4(result, 1.0);
      
      
}