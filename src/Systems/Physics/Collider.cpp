#include "Collider.hpp"
#include "../../Components/PhysicsComponent.hpp"

Collider::Collider( int entityID,
                    glm::vec3 center,
                    ColliderType colliderType,
                    DynamicType dynamicType) : \
                    entityID(entityID),
                    center(center), 
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

void Collider::ConvexHull()
{

}