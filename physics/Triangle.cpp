#include "Triangle.hpp"

Triangle::Triangle(glm::vec3 center, glm::vec3 normal, std::vector<glm::vec3> points, bool dynamic, ColliderType colliderType) : 
            Collider(center, dynamic, colliderType), normal(normal), points(points)
{

}

bool Triangle::CheckCollision(std::shared_ptr<Triangle> triangle)
{
    // https://cis.temple.edu/~lakaemper/courses/cis350_2004/etc/moeller_triangle.pdf
}

bool Triangle::CheckCollision(std::shared_ptr<Sphere> sphere)
{
    // if the distance between the plane of the triangle and the center of the 
    // sphere is less than the radius, then they intersect.
    
}

bool Triangle::CheckCollision(std::shared_ptr<AABB> box)
{
    return box->CheckCollision(this->shared_from_this());
}


bool Triangle::CheckCollision(std::shared_ptr<Collider> collider)
{
    if (collider->type == ColliderType::BOX)
    {
        std::shared_ptr<AABB> ptr = std::static_pointer_cast<AABB>(collider);
        return this->CheckCollision(ptr);
    }
    else if (collider->type == ColliderType::TRIANGLE)
    {
        std::shared_ptr<Triangle> ptr = std::static_pointer_cast<Triangle>(collider);
        return this->CheckCollision(ptr);
    }
    else
    {
        std::shared_ptr<Sphere> ptr = std::static_pointer_cast<Sphere>(collider);
        return this->CheckCollision(ptr);
    }
}