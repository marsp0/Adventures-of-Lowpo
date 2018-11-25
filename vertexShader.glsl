#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 viewPosition;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
// uniform mat4 lightSpaceMatrix;

out vec3 normal;
out vec3 fragPosition;
out vec2 TexCoord;
// out vec4 fragPosLightSpace;

void main()
{
    fragPosition = vec3(model * vec4(aPos,1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y); 
    // fragPosLightSpace = lightSpaceMatrix * model * vec4(fragPosition, 1.0);
}