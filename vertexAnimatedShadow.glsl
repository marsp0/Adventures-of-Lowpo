#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 boneIndices;
layout (location = 4) in vec4 weights;

const int MAX_BONES = 16;

uniform mat4 offsets[MAX_BONES];
uniform mat4 gBones[MAX_BONES];

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    uvec4 indices = uvec4(boneIndices);
    mat4 boneTransform   = gBones[indices[0]] * offsets[indices[0]] * weights[0];
    boneTransform       += gBones[indices[1]] * offsets[indices[1]] * weights[1];
    boneTransform       += gBones[indices[2]] * offsets[indices[2]] * weights[2];
    boneTransform       += gBones[indices[3]] * offsets[indices[3]] * weights[3];

    gl_Position = lightSpaceMatrix * model * boneTransform * vec4(aPos, 1.0);
}