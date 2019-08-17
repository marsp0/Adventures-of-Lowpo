#include "Collider.hpp"
#include "../../Components/PhysicsComponent.hpp"

Collider::Collider( int id,
                    glm::vec3 center, 
                    std::string name,
                    ColliderType colliderType,
                    DynamicType dynamicType) : \
                    entityID(id),
                    center(center), 
                    name(name),
                    colliderType(colliderType),
                    dynamicType(dynamicType)
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

void Collider::Update(glm::vec3 translation)
{
    this->center += translation;
    this->ComputeDerivedData();
}

DynamicType Collider::GetType()
{
    return this->dynamicType;
}