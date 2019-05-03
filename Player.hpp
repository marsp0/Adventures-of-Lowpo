#pragma once
#include "Shader.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Animator.hpp"

enum PlayerActions
{
    MOVE_FOREWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP
};
class Player : public GameObject
{
    public:
        Player( std::shared_ptr<Texture>    texture, 
                std::shared_ptr<Mesh>       mesh,
                std::shared_ptr<Animator>   animator,
                std::shared_ptr<RigidBody>  rigidBody,
                std::string                 shader,
                std::string                 shadowShader,
                float                       cameraWidth,
                float                       cameraHeight);

        virtual void    HandleInput(GLFWwindow* window);
        virtual void    Render(std::shared_ptr<Shader> shader);
        void            Update(float deltaTime);
        std::shared_ptr<Camera> GetCamera();

    private:

        std::shared_ptr<Camera> camera;

        // Input
        bool    actions[5];
        double  lastPositionX;
        double  lastPositionY;

        // direction
        // its used when moving
        // moving left and right means rotating the direction
        // moving camera means moving the direction
        glm::vec3   direction;
        float       speed;

        // Animations
        std::shared_ptr<Animator> animator;
};