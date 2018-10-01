#pragma once

#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Terrain.hpp"

class PhysicsEngine
{
    public:
        
        PhysicsEngine();

        void Step(float deltaTime, std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<Terrain> terrain);

};