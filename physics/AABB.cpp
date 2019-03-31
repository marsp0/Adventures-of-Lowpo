#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 center, glm::vec3 axisRadii, std::vector<glm::vec3> points, ColliderType colliderType, DynamicType dynamicType) : 
                        Collider(center, points, colliderType, dynamicType), axisRadii(axisRadii)
{
    // compute edges
    glm::vec3 a = this->points[0];
    glm::vec3 b = this->points[1];
    glm::vec3 c = this->points[2];
    glm::vec3 d = this->points[3];
    glm::vec3 e = this->points[4];
    glm::vec3 f = this->points[5];
    glm::vec3 g = this->points[6];
    glm::vec3 h = this->points[7];
    this->edges.push_back(std::make_pair(a, b));
    this->edges.push_back(std::make_pair(a, d));
    this->edges.push_back(std::make_pair(a, e));
    this->edges.push_back(std::make_pair(b, c));
    this->edges.push_back(std::make_pair(b, f));
    this->edges.push_back(std::make_pair(c, d));
    this->edges.push_back(std::make_pair(c, g));
    this->edges.push_back(std::make_pair(d, h));
    this->edges.push_back(std::make_pair(e, f));
    this->edges.push_back(std::make_pair(e, h));
    this->edges.push_back(std::make_pair(f, g));
    this->edges.push_back(std::make_pair(g, h));

    // compute faces

    glm::vec3 x = glm::vec3(1.f,0.f,0.f);
    glm::vec3 y = glm::vec3(0.f,1.f,0.f);
    glm::vec3 z = glm::vec3(0.f,0.f,1.f);

    float distance = x.x * (- b.x) + x.y * (- b.y) + x.z * (- b.z);
    this->faces.push_back(std::make_pair(x, distance));
    distance = -x.x * (- a.x) - x.y * (- a.y) - x.z * (- a.z);
    this->faces.push_back(std::make_pair(-x, distance));

    distance = y.x * (- e.x) + y.y * (- e.y) + y.z * (- e.z);
    this->faces.push_back(std::make_pair(y, distance));
    distance = -y.x * (- a.x) - y.y * (- a.y) - y.z * (- a.z);
    this->faces.push_back(std::make_pair(-y, distance));

    distance = z.x * (- b.x) + z.y * (- b.y) + z.z * (- b.z);
    this->faces.push_back(std::make_pair(z, distance));
    distance = -z.x * (- a.x) - z.y * (- a.y) - z.z * (- a.z);
    this->faces.push_back(std::make_pair(-y, distance));
}