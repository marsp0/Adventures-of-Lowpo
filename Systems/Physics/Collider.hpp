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
        center - the center point of the collider
        dynamic - should the collider be treated as static environment or a normal object
        that can be rotated/translated etc when forces are applied to it.
        */
        Collider( glm::vec3 center,
                  std::string name,
                  std::shared_ptr<PhysicsComponent> component,   
                  ColliderType colliderType);

        virtual ~Collider();

        const std::vector<glm::vec3>&                       GetPoints();
        const std::vector<glm::vec3>&                       GetPointsOnFaces();
        const std::vector<std::pair<glm::vec3, float>>&     GetFaces();
        const std::vector<std::pair<glm::vec3, glm::vec3>>& GetEdges();

        std::shared_ptr<PhysicsComponent>                   GetParent();
        void                                                SetParent(std::shared_ptr<PhysicsComponent> component);

        virtual void                                        ComputeDerivedData() = 0;
        void                                                Update(glm::vec3 position);
        DynamicType                                         GetType();
        glm::vec3    center;
        std::string  name;
        ColliderType colliderType;

    protected:

        std::vector<glm::vec3>      points;
        std::vector<glm::vec3>      pointsOnFaces;
        std::shared_ptr<PhysicsComponent>  component;

        std::vector<std::pair<glm::vec3, float>>     faces;
        std::vector<std::pair<glm::vec3, glm::vec3>> edges;
};