#include "Bone.hpp"

Bone::Bone(const int boneIndex, const glm::mat4& offsetMatrix, const std::string& name):
        boneIndex(boneIndex),
        offsetMatrix(offsetMatrix), name(name), localAnimationTransform(1.0), animationTransform(1.0)
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