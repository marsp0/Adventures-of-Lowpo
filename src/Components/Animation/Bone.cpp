#include "Bone.hpp"
#include <iostream>

Bone::Bone( int parentIndex,
            glm::mat4 offsetMatrix,
            glm::mat4 toParentSpace,
            std::string name):
        name(name), 
        parentIndex(parentIndex),
        animationTransform(1.0),
        offsetMatrix(offsetMatrix), 
        toParentSpace(toParentSpace),
        localAnimationTransform(1.0) 
{

}