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

void PhysicsEngine::HandleCollisions(std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<Terrain> terrain)
{
    // start with an O(n^2) and then work on optimizing it.
    for (int i = 0 ; i < gameObjects.size(); i++)
    {
        glm::vec3 objectPosition = gameObjects[i]->physicsComponent.GetPosition();
        gameObjects[i]->physicsComponent.position.y = terrain->GetHeight(objectPosition.x, objectPosition.z);        
    }
}