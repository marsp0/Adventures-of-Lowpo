#include "Collider.hpp"
#include "../../Components/PhysicsComponent.hpp"
#include "../../util.hpp"

Collider::Collider( int entityID,
                    glm::vec3 center,
                    std::vector<glm::vec3> points,
                    std::vector<std::pair<int, int>> edges,
                    std::vector<ColliderFace> faces,
                    DynamicType  dynamicType) : \
                    row(0),
                    col(0),
                    center(center),
                    faces(faces),
                    edges(edges),
                    points(points),
                    entityID(entityID),
                    dynamicType(dynamicType)
{

}

Collider::~Collider()
{
    
}

void Collider::Update(glm::vec3 translation)
{
    this->center += translation;
    for (int i = 0; i < this->points.size(); i++)
    {
        this->points[i] += translation;
    }
    printVector(this->center, "Collider center");
}

const std::vector<glm::vec3>& Collider::GetPoints()
{
    assert(this->points.size() > 0);
    return this->points;
}

const std::vector<ColliderFace>& Collider::GetFaces()
{
    assert(this->faces.size() > 0);
    return this->faces;
}

const std::vector<std::pair<int, int>>& Collider::GetEdges()
{
    assert(this->edges.size() > 0);
    return this->edges;
}