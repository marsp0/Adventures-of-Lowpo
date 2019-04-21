#include "Collider.hpp"

Collider::Collider( glm::vec3 center, 
                    std::shared_ptr<RigidBody> rigidBody, 
                    ColliderType colliderType, 
                    DynamicType dynamicType) : \
                    center(center), 
                    colliderType(colliderType), 
                    dynamicType(dynamicType),
                    rigidBody(rigidBody)
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

const std::vector<glm::vec3>& Collider::GetPointsOnFaces()
{
    assert(this->pointsOnFaces.size() > 0);
    return this->pointsOnFaces;
}

std::shared_ptr<RigidBody> Collider::GetParent()
{
    return this->rigidBody;
}

void Collider::Update(glm::vec3 position)
{
    this->center += position;
    this->ComputeDerivedData();
}