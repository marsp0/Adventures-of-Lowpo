#pragma once

#include "Bone.hpp"
#include <unordered_map>

class Skeleton
{
    public:

        Skeleton();

        void                    AddBone(glm::mat4& offsetMatrix, std::string& name);
        std::shared_ptr<Bone>   GetBone(std::string& name);
        void                    UpdateBones();
        void                    UpdateBones(std::shared_ptr<BoneTreeNode>& node, const glm::mat4& parentMatrix);
        
    private:


        std::vector<std::shared_ptr<Bone>>      bones;
        BoneTreeNode                            root;
        glm::mat4                               rootInverseTransform;
        std::unordered_map<std::string, int>    boneNameToIndex;
};