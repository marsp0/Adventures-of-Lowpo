#pragma once

#include <glm/glm.hpp>

enum ColliderType
{
    TRIANGLE,
    SPHERE,
    AABOX
};

class Triangle;
class Sphere;
class AABB;

class Collider
{
    public:

        Collider(glm::vec3 center, bool dynamic);
        virtual ~Collider();

        virtual bool CheckCollision(Triangle triangle) = 0;
        virtual bool CheckCollision(Sphere sphere) = 0;
        virtual bool CheckCollision(AABB box) = 0;

        glm::vec3 center;
        bool dynamic;
};