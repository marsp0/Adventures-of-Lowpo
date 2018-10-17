#version 330 core


in vec3 normal;
in vec3 fragPosition;
in vec2 TexCoord;
in vec4 fragPosLightSpace;

out vec4 FragColor;

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


void main()
{
	// ambient 
	vec3 ambient = light.ambient * texture(texture1, TexCoord).rgb;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * texture(texture1, TexCoord).rgb;
	
	float shadow = ShadowCalculation(fragPosLightSpace);
	vec3 result = ambient + (1.0 - shadow) * diffuse;
	// vec3 result = vec3(1.0,0.0,0.5);
	FragColor = vec4(result, 1.0);
      
}