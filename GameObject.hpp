#pragma once

#include <memory>

#include "Texture.hpp"
#include "ObjectTransform.hpp"
#include "Mesh.hpp"
#include "PhysicsComponent.hpp"

class GameObject
{
    public:
        
        // GameObject(Transform transform, std::shared_ptr<Texture> texture, std::shared_ptr<Mesh> mesh, PhysicsComponent physicsComponent);
        GameObject(Transform transform, std::shared_ptr<Texture> texture, std::shared_ptr<Mesh> mesh);

        void HandleInput();
        void Update(float deltaTime);
        void Render();

        Transform                   transform;
        // PhysicsComponent            physicsComponent;
        std::shared_ptr<Texture>    texture;
        std::shared_ptr<Mesh>       mesh;
};