#pragma once

#include "GameObject.hpp"
#include "Camera.hpp"

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
        Player( Transform                   transform, 
                std::shared_ptr<Texture>    texture, 
                std::shared_ptr<Mesh>       mesh,
                PhysicsComponent physicsComponent,
                float cameraWidth,
                float cameraHeight);

        virtual void    HandleInput(GLFWwindow* window);
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
};