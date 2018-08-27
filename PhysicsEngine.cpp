#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine()
{

}

void PhysicsEngine::Step(float deltaTime, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    for (int i = 0 ; i < gameObjects.size() ; i++)
    {
        gameObjects[i]->physicsComponent.Integrate(deltaTime);
        
    }
}

void PhysicsEngine::HandleCollisions(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    // start with an O(n^2) and then work on optimizing it.
}