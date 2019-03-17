#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

enum ColliderType
{
    TRIANGLE,
    BOX,
    PLANE
};

enum DynamicType
{
    STATIC,
    DYNAMIC,
    DYNAMIC_LIVE
};

/* 
Collider is the base class of all colliders in the engine.
declares the behaviour/data that every collider should have.
*/
class Collider
{
    public:
        /* 
        center - the center point of the collider
        dynamic - should the collider be treated as static environment or a normal object
        that can be rotated/translated etc when forces are applied to it.
        */
        Collider(glm::vec3 center, ColliderType colliderType, DynamicType dynamicType);
        virtual ~Collider();

        virtual std::vector<glm::vec3> GetPoints() = 0;

        ColliderType    colliderType;
        DynamicType     dynamicType;
        glm::vec3       center;
};