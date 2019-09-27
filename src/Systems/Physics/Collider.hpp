#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../../Components/PhysicsComponent.hpp"

enum ColliderType
{
    BOX,
    PLANE
};

/* 
Collider - currently represents either a box or plane. Both use the same interface.
*/
class Collider
{
    public:
        /* 
        entityID     - the ID of the entity that owns the collider.
        center       - the center point of the collider
        colliderType - runtime type info.
        dynamic      - enum indicating how we should treat the collider during collisions
        */
        Collider( int entityID,
                  glm::vec3 center,
                  ColliderType colliderType,
                  DynamicType  dynamicType);

        virtual ~Collider();
        virtual void ComputeDerivedData() = 0;
        virtual void Update(glm::vec3 translation) = 0;

        // ACCESSORS

        const std::vector<glm::vec3>& GetPoints();
        const std::vector<glm::vec3>& GetPointsOnFaces();
        const std::vector<std::pair<glm::vec3, float>>&     GetFaces();
        const std::vector<std::pair<glm::vec3, glm::vec3>>& GetEdges();

        int                         row;
        int                         col;
        std::string                 name;
        glm::vec3                   center;
        int                         entityID;
        DynamicType                 dynamicType;
        ColliderType                colliderType;

    protected:

        std::vector<glm::vec3>      points;
        std::vector<glm::vec3>      pointsOnFaces;

        std::vector<std::pair<glm::vec3, float>>     faces;
        std::vector<std::pair<glm::vec3, glm::vec3>> edges;
};