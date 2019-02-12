#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 center, float radius) : BoundingVolume(center), radius(radius)
{

}

Sphere::~Sphere()
{

}

bool Sphere::CheckCollision(Triangle triangle)
{
    // https://cis.temple.edu/~lakaemper/courses/cis350_2004/etc/moeller_triangle.pdf
}

bool Sphere::CheckCollision(Sphere sphere)
{
    // if the distance between the plane of the triangle and the center of the 
    // sphere is less than the radius, then they intersect.
    
}

bool Sphere::CheckCollision(BoundingBox box)
{

}