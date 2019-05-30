#include "Collider.hpp"
#include "../../Components/PhysicsComponent.hpp"

Collider::Collider( glm::vec3 center, 
                    std::string name,
                    std::shared_ptr<PhysicsComponent> component, 
                    ColliderType colliderType) : \
                    center(center), 
                    name(name),
                    colliderType(colliderType), 
                    component(component)
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

std::shared_ptr<PhysicsComponent> Collider::GetParent()
{
    return this->component;
}

void Collider::SetParent(std::shared_ptr<PhysicsComponent> component)
{
    this->component = component;
}

void Collider::Update(glm::vec3 position)
{
    this->center += position;
    this->ComputeDerivedData();
}

DynamicType Collider::GetType()
{
    return this->component->dynamicType;
}