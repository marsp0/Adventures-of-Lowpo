#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Collider.hpp"

/* 
Triangle Collider
Points - {a,b,c}
    // b
    // |\
    // | \ a
    // | /
    // |/
    // c
 */
class Triangle : public Collider
{
    public:
        Triangle(   glm::vec3               center, 
                    glm::vec3               normal, 
                    std::vector<glm::vec3>  points, 
                    ColliderType            colliderType,
                    DynamicType             dynamicType);

        // DATA
        glm::vec3 normal;
    private:
};