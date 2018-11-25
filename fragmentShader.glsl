#version 330 core


in vec3 normal;
in vec3 fragPosition;
in vec2 TexCoord;

out vec4 FragColor;

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

uniform sampler2D texture1;


void main()
{
	// ambient 
	vec3 ambient = light.ambient * texture(texture1, TexCoord).rgb;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * texture(texture1, TexCoord).rgb;
	
	vec3 result = ambient + diffuse;
	FragColor = vec4(result, 1.0);
}