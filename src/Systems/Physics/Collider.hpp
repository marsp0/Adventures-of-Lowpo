#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../../Components/PhysicsComponent.hpp"


struct ColliderFace
{
    glm::vec3 normal;
    std::vector<int> points;
};

/**
Collider - currently represents either a box or plane. Both use the same interface.
*/
class Collider
{
    public:
        /**
        entityID     - the ID of the entity that owns the collider.
        center       - the center point of the collider
        colliderType - runtime type info.
        dynamic      - enum indicating how we should treat the collider during collisions
        */
        Collider( int entityID,
                  glm::vec3 center,
                  std::vector<glm::vec3> points,
                  std::vector<std::pair<int, int>> edges,
                  std::vector<ColliderFace> faces,
                  DynamicType  dynamicType);

        ~Collider();
        // virtual void ComputeDerivedData() = 0;
        void Update(glm::vec3 translation);

        // ACCESSORS

        const std::vector<glm::vec3>&           GetPoints();
        const std::vector<ColliderFace>&        GetFaces();
        const std::vector<std::pair<int, int>>& GetEdges();

        int                         row;
        int                         col;
        glm::vec3                   center;
        int                         entityID;
        DynamicType                 dynamicType;

    protected:

        std::vector<glm::vec3>              points;
        std::vector<ColliderFace>           faces;
        std::vector<std::pair<int, int>>    edges;
};