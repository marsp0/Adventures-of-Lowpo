#include "Bone.hpp"
#include <iostream>

Bone::Bone( int boneIndex,
            glm::mat4 offsetMatrix,
            glm::mat4 toParentSpace,
            std::string name):
        name(name), 
        boneIndex(boneIndex),
        animationTransform(1.0),
        offsetMatrix(offsetMatrix), 
        toParentSpace(toParentSpace),
        localAnimationTransform(1.0) 
{

}