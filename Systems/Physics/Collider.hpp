#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../../Components/PhysicsComponent.hpp"

enum ColliderType
{
    TRIANGLE,
    BOX,
    PLANE
};

/* 
Collider is the base class of all colliders in the engine.
declares the behaviour/data that every collider should have.
*/
class Collider
{
    public:
        /* 
        center - the center point of the collidera
        dynamic - should the collider be treated as static environment or a normal object
        that can be rotated/translated etc when forces are applied to it.
        */
        Collider( int entityID,
                  glm::vec3 center,
                  std::string name,
                  ColliderType colliderType,
                  DynamicType  dynamicType);

        virtual ~Collider();

        const std::vector<glm::vec3>&                       GetPoints();
        const std::vector<glm::vec3>&                       GetPointsOnFaces();
        const std::vector<std::pair<glm::vec3, float>>&     GetFaces();
        const std::vector<std::pair<glm::vec3, glm::vec3>>& GetEdges();

        virtual void                                        ComputeDerivedData() = 0;
        void                                                Update(glm::vec3 translation);
        DynamicType                                         GetType();

        int          row;
        int          col;
        std::string  name;
        glm::vec3    center;
        int          entityID;
        ColliderType colliderType;
        DynamicType  dynamicType;

    protected:

        std::vector<glm::vec3>      points;
        std::vector<glm::vec3>      pointsOnFaces;

        std::vector<std::pair<glm::vec3, float>>     faces;
        std::vector<std::pair<glm::vec3, glm::vec3>> edges;
};