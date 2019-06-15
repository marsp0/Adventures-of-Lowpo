#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>
#include "../Messaging/Message.hpp"

class Entity;
class InputSystem
{
    public:
        InputSystem();
        ~InputSystem();

        void Update(GLFWwindow* window, 
        			std::vector<std::shared_ptr<Entity>>& entities, 
        			std::vector<Message>& messages,
        			std::vector<Message>& globalQueue);
    private:
        std::uint32_t primaryBitset;
};  