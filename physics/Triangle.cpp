#include "Triangle.hpp"

#include <iostream>

Triangle::Triangle( glm::vec3 center, 
                    glm::vec3 normal, 
                    std::vector<glm::vec3> points, 
                    ColliderType colliderType,
                    DynamicType dynamicType) : 
            Collider(center, points, colliderType, dynamicType), normal(normal)
{
    // TODO : Decide if the normal will be passed or generated on the fly by the points passed
    if (glm::dot(glm::vec3(0.f,1.f,0.f), this->normal) < 0)
    {
        this->normal = -this->normal;
    }
    this->ComputeDerivedData();
}

void Triangle::ComputeDerivedData()
{
    // compute edges
    glm::vec3 a = this->points[0];
    glm::vec3 b = this->points[1];
    glm::vec3 c = this->points[2];
    this->edges.push_back(std::make_pair(a, b));
    this->edges.push_back(std::make_pair(a, c));
    this->edges.push_back(std::make_pair(b, c));

    // compute face
    // using the formula - nx(x - x0) + ny(y - y0) + nz(z - z0)
    // where n is the normal
    // x0, y0, z0 is a point on the plane
    float distance = normal.x * (- a.x) + normal.y * (- a.y) + normal.z * (- a.z);
    this->faces.push_back(std::make_pair(normal, distance)); 
}