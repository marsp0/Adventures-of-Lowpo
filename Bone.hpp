#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class Bone 
{
    public:
        // METHODS
        Bone(const int boneIndex,const glm::mat4& offsetMatrix, const glm::mat4& toParentSpace, const std::string& name);

        void SetBoneIndex(int index);
        int GetBoneIndex();

        glm::mat4   GetOffsetMatrix();
        glm::mat4   GetParentSpaceTransform();
        void        SetAnimationTransform(const glm::mat4& transform);
        glm::mat4   GetAnimationTransform();
        glm::mat4   GetLocalAnimationTransform();
        void        SetLocalAnimationTransform(const glm::mat4& transform);


        std::string GetName();

    private:
        // DATA MEMBERS
        int         boneIndex;
        // offsetMatrix
        // the skeleton comes with a default bind pose
        // if we render the model as it is in its default pose we will get 
        // a regular looking mesh
        // that means that the whole VBO of vertex data is in model space
        // To be able to apply transformations to the bones you need them in bone space
        // To get to that bone space you need to multiply the bone by the inverseBindPose
        glm::mat4   offsetMatrix;
        // transforms the bone into the parent space
        glm::mat4   toParentSpace;
        glm::mat4   localAnimationTransform;
        glm::mat4   animationTransform;
        // Name of the bone that we can use to refer to it.
        // we can use a hash table to store name -> index
        std::string name;
};

struct BoneTreeNode
{
    int boneIndex;
    std::vector<std::shared_ptr<BoneTreeNode>> children;
};