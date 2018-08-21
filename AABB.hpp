#pragma once

#include <glm/glm.hpp>

class AABB
{
    public:
    
        AABB(glm::vec3 min, glm::vec3 max);
        bool HandleCollision(const AABB& other);
    
    private:

        glm::vec3 min;
        glm::vec3 max;
        
};