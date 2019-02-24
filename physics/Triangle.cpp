#include "Triangle.hpp"

Triangle::Triangle(glm::vec3 center, glm::vec3 normal, std::vector<glm::vec3> points, bool dynamic) : Collider(center, dynamic), normal(normal), points(points)
{

}

bool Triangle::CheckCollision(Triangle triangle)
{
    // https://cis.temple.edu/~lakaemper/courses/cis350_2004/etc/moeller_triangle.pdf
}

bool Triangle::CheckCollision(Sphere sphere)
{
    // if the distance between the plane of the triangle and the center of the 
    // sphere is less than the radius, then they intersect.
    
}

bool Triangle::CheckCollision(AABB box)
{

}