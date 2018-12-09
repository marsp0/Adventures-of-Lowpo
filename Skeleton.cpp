#include "Skeleton.hpp"

Skeleton::Skeleton()
{

}

void Skeleton::AddBone(glm::mat4& offsetMatrix, std::string& name)
{
    std::shared_ptr<Bone> bone = std::make_shared<Bone>(this->bones.size(), offsetMatrix, name );
    this->bones.push_back(bone);
    this->boneNameToIndex[name] = bone->GetBoneIndex();
}

std::shared_ptr<Bone> Skeleton::GetBone(std::string& name)
{
    return this->bones[this->boneNameToIndex[name]];
}

void Skeleton::UpdateBones()
{
    for (int i = 0; i < this->root.children.size() ; i++)
    {
        this->UpdateBones(this->root.children[i], rootInverseTransform);
    }
}

void Skeleton::UpdateBones(std::shared_ptr<BoneTreeNode>& node, const glm::mat4& parentMatrix)
{
    glm::mat4 aMatrix = parentMatrix * this->bones[node->boneIndex]->GetLocalAnimationTransform();
    this->bones[node->boneIndex]->SetAnimationTransform(aMatrix);
    for (int i = 0; i < node->children.size(); i++)
    {
        this->UpdateBones(node->children[i],aMatrix);
    }
}