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

uniform Light 		light;
uniform sampler2D 	mainTexture;
uniform sampler2D 	shadowMap; 

float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	// apply bias to avoid acne
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005); 
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}


void main()
{
	// ambient 
	vec3 ambient = light.ambient * texture(mainTexture, TexCoord).rgb;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);

	// float diff = max(dot(norm,lightDir),0.0);
	float diff = max(dot(norm,lightDir) * 0.5 + 0.5, 0.0);
	vec3 diffuse = light.diffuse * diff * texture(mainTexture, TexCoord).rgb;
	// vec3 diffuse = diff * texture(mainTexture, TexCoord).rgb;
	
	float shadow = ShadowCalculation(fragPosLightSpace, norm, lightDir);
	vec3 result = ambient + (1.0 - shadow) * diffuse;
	FragColor = vec4(result, 1.0);
}