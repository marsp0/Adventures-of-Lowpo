#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoord;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 ambientColor;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    fragPosition = vec3(model * vec4(aPos,1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    // normal = vec3(1.0,1.0,0.0);
    // normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}