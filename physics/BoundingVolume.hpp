#pragma once

#include <glm/glm.hpp>

class Triangle;
class Sphere;
class BoundingBox;

class BoundingVolume
{
    public:

        BoundingVolume(glm::vec3 center);
        virtual ~BoundingVolume();

        virtual bool CheckCollision(Triangle triangle) = 0;
        virtual bool CheckCollision(Sphere sphere) = 0;
        virtual bool CheckCollision(BoundingBox box) = 0;

        glm::vec3 center;
};