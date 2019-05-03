
#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "physics/RigidBody.hpp"
// stays here because glew should get included before glfw
// glew is present in GameObject.hpp
// TODO : fix this mess
#include <GLFW/glfw3.h>


/* 
Represents a single entity in the game.
 */
class GameObject
{
    public:
        
        GameObject( std::shared_ptr<Texture>    texture, 
                    std::shared_ptr<Mesh>       mesh,
                    std::shared_ptr<RigidBody>  rigidBody,
                    std::string shader,
                    std::string shadowShader);

        virtual void HandleInput(GLFWwindow* window);
        virtual void Update(float deltaTime);
        virtual void Render(std::shared_ptr<Shader> shader);
        glm::mat4    GetWorldMatrix();
        glm::vec3    GetPosition();

        std::shared_ptr<RigidBody>  rigidBody;
        std::shared_ptr<Texture>    texture;
        // Question : Does it matter if the mesh is a ptr or not ?
        std::shared_ptr<Mesh>       mesh;
        std::string                 shader;
        std::string                 shadowShader;
};