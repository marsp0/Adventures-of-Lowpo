#include "PhysicsSystem.hpp"

#include "../../Components/PhysicsComponent.hpp"

PhysicsSystem::PhysicsSystem(float gridLength, float cellHalfWidth) : grid(gridLength, cellHalfWidth)
{
    this->primaryBitset = ComponentType::Physics | ComponentType::Transform;
}

PhysicsSystem::~PhysicsSystem()
{
    
}

void PhysicsSystem::Init(std::vector<std::shared_ptr<Collider>>& colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        this->grid.Insert(colliders[i]);
    }
}

void PhysicsSystem::Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
{
    // Integration step
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->primaryBitset))
        {
            // acceleration update
            PhysicsComponent component = entities[i]->GetComponent<PhysicsComponent>(ComponentType::Physics);
            component.acceleration += component.forceAccumulator * component.inverseMass;
            component.angularAcc += component.invInertiaTensor * component.torqueAccumulator;
            // velocity update
            component.velocity += component.acceleration * deltaTime;
            component.angularVel += component.angularAcc * deltaTime;
            // position update
            component.position += component.velocity * deltaTime;
            component.orientation.x += 0.5f * component.orientation.x * component.angularVel.x * deltaTime;
            component.orientation.y += 0.5f * component.orientation.y * component.angularVel.y * deltaTime;
            component.orientation.z += 0.5f * component.orientation.z * component.angularVel.z * deltaTime;
        }
    }
    // 2. Check for collision
    std::vector<std::shared_ptr<Collision>> collisions = this->grid.CheckCollisions();
    // 3. Resolve Collisions
    this->collisionResolver.Solve(collisions);
    // 4. Resolve Interpenetration
    // TO DO
}