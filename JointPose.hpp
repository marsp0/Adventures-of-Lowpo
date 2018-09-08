#pragma once

#include <glm/glm.hpp>

class JointPose
{
    public:
        JointPose();
        ~JointPose();

        // glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 scale;


};