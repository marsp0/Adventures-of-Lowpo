#pragma once

#include <glm/glm.hpp>

class Contact
{
    public:
        // what we need
        // Collision Point
        glm::vec3 contactPoint;
        // Contact Normal
        glm::vec3 contactNormal;
        // Penetration Depth
        float penetration;
    private:
};