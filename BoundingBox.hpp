#pragma once

#include <glm/glm.hpp>

enum ObjectType
{
    Static,
    Dynamic
};

class BoundingBox
{
    public:
    
        // Methods
        BoundingBox(glm::vec3 min, glm::vec3 max, ObjectType type);
        void Update(glm::vec3 position);
        bool HandleCollision(const BoundingBox& other);
        
        // DATA
        ObjectType  type;
    
    private:

        glm::vec3   min;
        glm::vec3   max;
};