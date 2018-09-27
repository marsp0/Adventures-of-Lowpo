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

void PhysicsEngine::HandleCollisions(float deltaTime, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<Terrain> terrain)
{
    // start with an O(n^2) and then work on optimizing it.
    for (int i = 0 ; i < gameObjects.size(); i++)
    {
        glm::vec3 objectPosition = gameObjects[i]->physicsComponent.GetPosition();
        gameObjects[i]->physicsComponent.position.y = terrain->GetHeight(objectPosition.x, objectPosition.z);        
    }

    for (int i = 0; i < gameObjects.size(); i++)
    {
        for (int j = i+1; j < gameObjects.size(); j++)
        {
            if (gameObjects[i]->physicsComponent.boundingBox.HandleCollision(gameObjects[j]->physicsComponent.boundingBox))
            {
                if (gameObjects[i]->physicsComponent.boundingBox.type == ObjectType::Dynamic)
                {
                    gameObjects[i]->SetVelocity(-.5f*gameObjects[i]->GetVelocity());
                    // gameObjects[i]->SetVelocity(glm::vec3(0.f,0.f,0.f));
                    gameObjects[i]->physicsComponent.canMove = false;
                } 
                else if (gameObjects[j]->physicsComponent.boundingBox.type == ObjectType::Dynamic)
                {
                    gameObjects[j]->SetVelocity(-gameObjects[j]->GetVelocity());
                    gameObjects[j]->physicsComponent.canMove = false;
                }
            }
        }
    }
}