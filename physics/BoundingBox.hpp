#pragma once

#include "Collider.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

class BoundingBox : public Collider
{
    public:

        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);

    private:

};