#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 center, glm::vec3 axisRadii, std::vector<glm::vec3> points, ColliderType colliderType, DynamicType dynamicType) : 
                        Collider(center, points, colliderType, dynamicType), axisRadii(axisRadii)
{
    this->ComputeDerivedData();
}

void AABB::ComputeDerivedData()
{
    // compute points
    this->points.clear();
    this->points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z - this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z - this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z + this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z + this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z - this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z - this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z + this->axisRadii.z));
    this->points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z + this->axisRadii.z));

    // compute edges
    this->edges.clear();
    this->edges.push_back(std::make_pair(this->points[0], this->points[1]));
    this->edges.push_back(std::make_pair(this->points[0], this->points[3]));
    this->edges.push_back(std::make_pair(this->points[0], this->points[4]));
    this->edges.push_back(std::make_pair(this->points[1], this->points[2]));
    this->edges.push_back(std::make_pair(this->points[1], this->points[5]));
    this->edges.push_back(std::make_pair(this->points[2], this->points[3]));
    this->edges.push_back(std::make_pair(this->points[2], this->points[6]));
    this->edges.push_back(std::make_pair(this->points[3], this->points[7]));
    this->edges.push_back(std::make_pair(this->points[4], this->points[5]));
    this->edges.push_back(std::make_pair(this->points[4], this->points[7]));
    this->edges.push_back(std::make_pair(this->points[5], this->points[6]));
    this->edges.push_back(std::make_pair(this->points[6], this->points[7]));

    // compute faces
    this->faces.clear();

    glm::vec3 x = glm::vec3(1.f,0.f,0.f);
    glm::vec3 y = glm::vec3(0.f,1.f,0.f);
    glm::vec3 z = glm::vec3(0.f,0.f,1.f);

    float distance = glm::dot(x, this->points[1]);
    this->faces.push_back(std::make_pair(x, distance));
    distance = glm::dot(-x, this->points[0]);
    this->faces.push_back(std::make_pair(-x, distance));

    distance = glm::dot(y, this->points[4]);
    this->faces.push_back(std::make_pair(y, distance));
    distance = glm::dot(-y, this->points[0]);
    this->faces.push_back(std::make_pair(-y, distance));

    distance = glm::dot(z, this->points[2]);
    this->faces.push_back(std::make_pair(z, distance));
    distance = glm::dot(-z, this->points[0]);
    this->faces.push_back(std::make_pair(-z, distance));

    // compute points on faces.
    this->pointsOnFaces.clear();
    this->pointsOnFaces.push_back(this->points[1]);
    this->pointsOnFaces.push_back(this->points[0]);
    this->pointsOnFaces.push_back(this->points[4]);
    this->pointsOnFaces.push_back(this->points[0]);
    this->pointsOnFaces.push_back(this->points[2]);
    this->pointsOnFaces.push_back(this->points[0]);
}