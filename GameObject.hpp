#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "ObjectTransform.hpp"
#include "Mesh.hpp"
#include "PhysicsComponent.hpp"
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
                    std::shared_ptr<Mesh>       mesh);

        void        HandleInput(GLFWwindow* window);
        void        Update(float deltaTime);
        void        Render();
        void        SetVelocity(glm::vec3 velocity);
        glm::vec3   GetVelocity();

        Transform                   transform;
        // PhysicsComponent            physicsComponent;
        std::shared_ptr<Texture>    texture;
        std::shared_ptr<Mesh>       mesh;
};