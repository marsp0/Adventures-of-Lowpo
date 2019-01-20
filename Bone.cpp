#include "Bone.hpp"
#include <iostream>

Bone::Bone(const int boneIndex, const glm::mat4& offsetMatrix, const glm::mat4& toParentSpace, const std::string& name):
        boneIndex(boneIndex),
        offsetMatrix(offsetMatrix), name(name), localAnimationTransform(1.0), animationTransform(1.0),
        toParentSpace(toParentSpace)
{

}

void Bone::SetBoneIndex(int index)
{
    this->boneIndex = index;
}

int Bone::GetBoneIndex()
{
    return this->boneIndex;
}

glm::mat4 Bone::GetOffsetMatrix()
{
    return this->offsetMatrix;
}

glm::mat4 Bone::GetParentSpaceTransform()
{
    return this->toParentSpace;
}

void Bone::SetAnimationTransform(const glm::mat4& transform)
{
    this->animationTransform = transform;
}

glm::mat4 Bone::GetAnimationTransform()
{
    return this->animationTransform;
}

void Bone::SetLocalAnimationTransform(const glm::mat4& transform)
{
    this->localAnimationTransform = transform;
}

glm::mat4   Bone::GetLocalAnimationTransform()
{
    return this->localAnimationTransform;
}

std::string Bone::GetName()
{
    return this->name;
}