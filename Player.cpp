#include "Player.hpp"
#include "glm/gtx/rotate_vector.hpp"

Player::Player( Transform                   transform,
                std::shared_ptr<Texture>    texture,
                std::shared_ptr<Mesh>       mesh,
                PhysicsComponent physicsComponent,
                float cameraWidth,
                float cameraHeight) :
                GameObject(transform, texture, mesh,physicsComponent),
                camera(cameraWidth, cameraHeight)
{

}

void Player::HandleInput(GLFWwindow* window)
{
    // KEYBOARD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->actions[PlayerActions::MOVE_FOREWARD] = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->actions[PlayerActions::MOVE_BACKWARD] = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->actions[PlayerActions::MOVE_LEFT] = true;;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->actions[PlayerActions::MOVE_RIGHT] = true;

    // // MOUSE
    // double xpos, ypos;
    // glfwGetCursorPos(window, &xpos, &ypos);
    // float xoffset = xpos - this->lastPositionX;
    // float yoffset = this->lastPositionY - ypos;

    // this->lastPositionX = xpos;
    // this->lastPositionY = ypos;

    // this->camera.HandleMouseInput(deltaTime,xoffset,yoffset);
}

void Player::Update(float deltaTime)
{
    // adjust velocity here if player is moving.
    for (int i = 0; i < 5; i++)
    {
        if (this->actions[i] == PlayerActions::MOVE_FOREWARD)
            // Proj of cam dir on xz plane
            this->SetVelocity(this->camera.direction - glm::dot(this->camera.direction,this->camera.worldUp)*this->camera.worldUp);
        if (this->actions[i] == PlayerActions::MOVE_BACKWARD)
            // negate proj cam dir on zn plane
            this->SetVelocity(-(this->camera.direction - glm::dot(this->camera.direction,this->camera.worldUp)*this->camera.worldUp));
        if (this->actions[i] == PlayerActions::MOVE_LEFT)
            // rotate velocity by 90 degrees
            this->SetVelocity(glm::rotate(this->GetVelocity(),glm::radians(90.f),this->camera.worldUp));
        if (this->actions[i] == PlayerActions::MOVE_RIGHT)
            // rotate velocity by -90 degrees
            this->SetVelocity(glm::rotate(this->GetVelocity(),glm::radians(-90.f),this->camera.worldUp));
        this->actions[i] = false;
    }
}