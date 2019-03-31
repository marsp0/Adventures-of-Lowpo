#include "Collider.hpp"

Collider::Collider(glm::vec3 center, std::vector<glm::vec3> points, ColliderType colliderType, DynamicType dynamicType) : center(center), colliderType(colliderType), dynamicType(dynamicType), points(points)
{
    
}

Collider::~Collider()
{
    
}

const std::vector<glm::vec3>& Collider::GetPoints()
{
    assert(this->points.size() > 0);
    return this->points;
}

const std::vector<std::pair<glm::vec3, float>>& Collider::GetFaces()
{
    assert(this->faces.size() > 0);
    return this->faces;
}

const std::vector<std::pair<glm::vec3, glm::vec3>>& Collider::GetEdges()
{
    assert(this->edges.size() > 0);
    return this->edges;
}