#pragma once

#include "Bone.hpp"
#include <unordered_map>

class Skeleton
{
    public:

        Skeleton(std::shared_ptr<BoneTreeNode> root, glm::mat4 rootInverseTransform);

        void                    AddBone(glm::mat4& offsetMatrix, glm::mat4& toParentSpace, std::string& name);
        std::shared_ptr<Bone>   GetBone(std::string& name);
        void                    UpdateBones();
        void                    UpdateBones(std::shared_ptr<BoneTreeNode>& node, const glm::mat4& parentMatrix);
        std::vector<glm::mat4>  GetOffsetMatrices();
        std::vector<glm::mat4>  GetAnimMatrices();
        
    private:


        std::vector<std::shared_ptr<Bone>>      bones;
        std::shared_ptr<BoneTreeNode>           root;
        glm::mat4                               rootInverseTransform;
        std::unordered_map<std::string, int>    boneNameToIndex;
};