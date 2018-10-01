#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine()
{

}

void PhysicsEngine::Step(float deltaTime, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<Terrain> terrain)
{
    // Integrate all the objects.
    for (int i = 0 ; i < gameObjects.size() ; i++)
    {
        gameObjects[i]->physicsComponent.Integrate(deltaTime);
    }

    // check for collisions
    for (int i = 0; i < gameObjects.size(); i++)
    {
        for (int j = i+1; j < gameObjects.size(); j++)
        {
            if (gameObjects[i]->physicsComponent.boundingBox.HandleCollision(gameObjects[j]->physicsComponent.boundingBox))
            {
                if (gameObjects[i]->physicsComponent.boundingBox.type == ObjectType::Dynamic)
                {
                    gameObjects[i]->physicsComponent.Integrate(-deltaTime);
                } 
                else if (gameObjects[j]->physicsComponent.boundingBox.type == ObjectType::Dynamic)
                {
                    gameObjects[j]->physicsComponent.Integrate(-deltaTime);
                }
            }
        }
    }

    for (int i = 0 ; i < gameObjects.size(); i++)
    {
        glm::vec3 objectPosition = gameObjects[i]->physicsComponent.GetPosition();
        gameObjects[i]->physicsComponent.position.y = terrain->GetHeight(objectPosition.x, objectPosition.z);
    }

}