#include "Skeleton.hpp"
#include <iostream>

Skeleton::Skeleton(std::shared_ptr<BoneTreeNode> root, glm::mat4 rootInverseTransform) : root(root), rootInverseTransform(rootInverseTransform)
{

}

void Skeleton::AddBone(glm::mat4& offsetMatrix, glm::mat4& toParentSpace, std::string& name)
{
    std::shared_ptr<Bone> bone = std::make_shared<Bone>(this->bones.size(), offsetMatrix, toParentSpace, name );
    this->bones.push_back(bone);
    this->boneNameToIndex[name] = bone->GetBoneIndex();
}

std::shared_ptr<Bone> Skeleton::GetBone(std::string& name)
{
    return this->bones[this->boneNameToIndex[name]];
}

void Skeleton::UpdateBones()
{
    // this->UpdateBones(this->root, this->rootInverseTransform);
    this->UpdateBones(this->root, glm::mat4(1.0f));
}

void Skeleton::UpdateBones(std::shared_ptr<BoneTreeNode>& node, const glm::mat4& parentMatrix)
{
    // glm::mat4 aMatrix = parentMatrix * this->bones[node->boneIndex]->GetParentSpaceTransform() * this->bones[node->boneIndex]->GetLocalAnimationTransform();
    glm::mat4 aMatrix = parentMatrix * this->bones[node->boneIndex]->GetLocalAnimationTransform();
    this->bones[node->boneIndex]->SetAnimationTransform(this->rootInverseTransform * aMatrix);
    for (int i = 0; i < node->children.size(); i++)
    {
        this->UpdateBones(node->children[i],aMatrix);
    }
}

std::vector<glm::mat4> Skeleton::GetOffsetMatrices()
{
    std::vector<glm::mat4> result;
    for (int i = 0; i < this->bones.size(); i++)
    {
        result.push_back(this->bones[i]->GetOffsetMatrix());
    }
    return result;
}

std::vector<glm::mat4> Skeleton::GetAnimMatrices()
{
    std::vector<glm::mat4> result;
    for (int i = 0 ; i < this->bones.size() ; i++)
    {
        result.push_back(this->bones[i]->GetAnimationTransform());
    }
    return result;
}