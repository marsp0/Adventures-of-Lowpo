#pragma once

#include <glm/glm.hpp>

class Triangle;
class Sphere;
class BoundingBox;

class Collider
{
    public:

        Collider(glm::vec3 center, bool dynamic);
        virtual ~Collider();

        virtual bool CheckCollision(Triangle triangle) = 0;
        virtual bool CheckCollision(Sphere sphere) = 0;
        virtual bool CheckCollision(BoundingBox box) = 0;

        glm::vec3 center;
        bool dynamic;
};