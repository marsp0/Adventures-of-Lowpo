
#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Mesh.hpp"
#include "PhysicsComponent.hpp"
#include "ObjectTransform.hpp"
#include "Material.hpp"
// stays here because glew should get included before glfw
// glew is present in GameObject.hpp
// TODO : fix this mess
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "RigidBody.hpp"

class GameObject
{
    public:
        
        GameObject( Transform                   transform, 
                    std::shared_ptr<Texture>    texture, 
                    std::shared_ptr<Mesh>       mesh,
                    RigidBody                   rigidBody,
                    std::string shader,
                    std::string shadowShader);

        virtual void    HandleInput(GLFWwindow* window);
        virtual void    Update(float deltaTime);
        virtual void    Render(std::shared_ptr<Shader> shader);

        Transform                   transform;
        RigidBody                   rigidBody;
        std::shared_ptr<Texture>    texture;
        std::shared_ptr<Mesh>       mesh;
        std::string                 shader;
        std::string                 shadowShader;
};