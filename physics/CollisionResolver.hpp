#pragma once

#include <glm/glm.hpp>

class CollisionResolver
{
    public:

        CollisionResolver();

        // Utilities

        /* 
        Generates orthonormal vectors from a given axis. 
        x - contact normal
        */
        void GenerateOrthonormalBasis(glm::vec3& x, glm::vec3& y, glm::vec3& z);
    private:
};