#include <iostream>
#include "InputSystem.hpp"
#include "../../Entity.hpp"
#include "../../Components/InputComponent.hpp"
#include "../Messaging/MoveData.hpp"
#include "../Messaging/MouseMoveData.hpp"

InputSystem::InputSystem() : actionList(4), generateKeyMessage(false), generateKeyRelease(false), generateMouseMessage(false), generateMouseRelease(false)
{
    this->primaryBitset = ComponentType::Input;
}

InputSystem::~InputSystem()
{

}

void InputSystem::Update(GLFWwindow* window, std::vector<std::shared_ptr<Entity>>& entities, std::vector<Message>& messages, std::vector<Message>& globalQueue)
{
    // Note : we probably want to add animation triggers here.
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->primaryBitset))
        {
            InputComponent* component = entities[i]->GetComponent<InputComponent>(ComponentType::Input);
            // KEYBOARD
            // TODO Martin: Fix this pile of shit.
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || \
                glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                this->actionList[Action::MoveForward]   = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? true : false;
                this->actionList[Action::MoveBackward]  = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? true : false;
                this->actionList[Action::MoveLeft]      = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? true : false;
                this->actionList[Action::MoveRight]     = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? true : false;
                this->generateKeyMessage = true;
                this->generateKeyRelease = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && \
                     glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE && \
                     this->generateKeyRelease)
            {
                this->generateKeyMessage = true;
            }   this->generateKeyRelease = false;

            if (this->generateKeyMessage)
            {
                Message message(entities[i]->id, 0, MessageType::Move);
                std::shared_ptr<MoveData> moveData = std::make_shared<MoveData>(this->actionList[Action::MoveForward],
                                                                                this->actionList[Action::MoveBackward],
                                                                                this->actionList[Action::MoveLeft],
                                                                                this->actionList[Action::MoveRight]);
                message.data = moveData;
                globalQueue.push_back(message);
                this->generateKeyMessage = false;
            }

            this->ClearActions();

            // Mouse
            // get mouse position
            double x, y;
            double deltaX;
            double deltaY;
            glfwGetCursorPos(window, &x, &y);
            // FIX : This needs to be made to work only for player.
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            {
                // disable mouse
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                deltaX = x - component->lastX;
                deltaY = y - component->lastY;
                this->generateMouseMessage = true;
                this->generateMouseRelease = true;
            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE && this->generateMouseRelease)
            {
                // enable mouse on release
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                this->generateMouseRelease = false;
            }
            // update components mouse coords.
            component->lastX = x;
            component->lastY = y;

            if (this->generateMouseMessage)
            {
                Message message(entities[i]->id, 0, MessageType::MouseMove);
                std::shared_ptr<MouseMoveData> mouseMoveData = std::make_shared<MouseMoveData>(deltaX, deltaY);
                message.data = mouseMoveData;
                globalQueue.push_back(message);
                this->generateMouseMessage = false;
            }
        }
    }
}

void InputSystem::ClearActions()
{
    for (int i = 0; i < this->actionList.size(); i++)
    {
        this->actionList[i] = false;
    }
}