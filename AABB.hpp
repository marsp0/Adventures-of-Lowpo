#pragma once

#include <glm/glm.hpp>

class AABB
{
    public:
    
        AABB(glm::vec3 min, glm::vec3 max);
        void Update(glm::vec3 position);
        bool HandleCollision(const AABB& other);
    
    private:

        glm::vec3 min;
        glm::vec3 max;
        
};