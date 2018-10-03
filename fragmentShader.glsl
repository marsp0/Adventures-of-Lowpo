#version 330 core

uniform vec3 viewPosition;
uniform vec3 diffuseColor;

in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;


void main()
{
	// ambient 
	vec3 ambient = light.ambient * diffuseColor;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	
	vec3 result = ambient + diffuse;
	FragColor = vec4(result, 1.0);
      
      
}