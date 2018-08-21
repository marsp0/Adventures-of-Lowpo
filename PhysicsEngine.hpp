#pragma once

#include <vector>
#include <memory>
#include "GameObject.hpp"

class PhysicsEngine
{
    public:
        
        PhysicsEngine();

        void Step(float deltaTime, std::vector<std::shared_ptr<GameObject>>& gameObjects);
        void HandleCollisions(std::vector<std::shared_ptr<GameObject>>& gameObjects);

};