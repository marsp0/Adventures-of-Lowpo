#pragma once

#include <vector>
#include <memory>

#include <PhysicsObject.hpp>

class PhysicsEngine
{
    public:
        
        PhysicsEngine();

        void AddPhysicsObject();
        void Step(float deltaTime);
        void HandleCollisions();

    private:

        std::vector<std::shared_ptr<PhysicsObject>> objects;
};