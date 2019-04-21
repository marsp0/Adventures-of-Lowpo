#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "RigidBody.hpp"

enum ColliderType
{
    TRIANGLE,
    BOX,
    PLANE
};

enum DynamicType
{
    Static,
    Dynamic,
    WithPhysics
};

/* 
Collider is the base class of all colliders in the engine.
declares the behaviour/data that every collider should have.
*/
class Collider
{
    public:
        /* 
        center - the center point of the collider
        dynamic - should the collider be treated as static environment or a normal object
        that can be rotated/translated etc when forces are applied to it.
        */
        Collider( glm::vec3 center,
                  std::shared_ptr<RigidBody> rigidBody,   
                  ColliderType colliderType, 
                  DynamicType dynamicType);

        virtual ~Collider();

        const std::vector<glm::vec3>& GetPoints();
        const std::vector<std::pair<glm::vec3, glm::vec3>>& GetEdges();
        const std::vector<std::pair<glm::vec3, float>>& GetFaces();
        const std::vector<glm::vec3>& GetPointsOnFaces();
        std::shared_ptr<RigidBody> GetParent();
        virtual void ComputeDerivedData() = 0;
        void Update(glm::vec3 position);

        glm::vec3                   center;
        DynamicType                 dynamicType;
        ColliderType                colliderType;

    protected:

        std::vector<glm::vec3>      points;
        std::vector<glm::vec3>      pointsOnFaces;
        std::shared_ptr<RigidBody>  rigidBody;

        std::vector<std::pair<glm::vec3, float>>     faces;
        std::vector<std::pair<glm::vec3, glm::vec3>> edges;
};