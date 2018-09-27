#pragma once

#include <glm/glm.hpp>

enum ObjectType
{
    Static,
    Dynamic
};

class AABB
{
    public:
    
        // Methods
        AABB(glm::vec3 min, glm::vec3 max, ObjectType type);
        void Update(glm::vec3 position);
        bool HandleCollision(const AABB& other);
        
        // DATA
        ObjectType  type;
    
    private:

        glm::vec3   min;
        glm::vec3   max;
};