
#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Mesh.hpp"
#include "PhysicsComponent.hpp"
#include "ObjectTransform.hpp"
// stays here because glew should get included before glfw
// glew is present in GameObject.hpp
// TODO : fix this mess
#include <GLFW/glfw3.h>

class GameObject
{
    public:
        
        // GameObject(Transform transform, std::shared_ptr<Texture> texture, std::shared_ptr<Mesh> mesh, PhysicsComponent physicsComponent);
        GameObject( Transform                   transform, 
                    std::shared_ptr<Texture>    texture, 
                    std::shared_ptr<Mesh>       mesh,
                    PhysicsComponent physicsComponent);

        virtual void    HandleInput(GLFWwindow* window);
        virtual void    Update(float deltaTime);
        void            Render();
        void            SetVelocity(glm::vec3 velocity);
        glm::vec3       GetVelocity();
        glm::vec3       GetPosition();

        Transform                   transform;
        PhysicsComponent            physicsComponent;
        std::shared_ptr<Texture>    texture;
        std::shared_ptr<Mesh>       mesh;
};