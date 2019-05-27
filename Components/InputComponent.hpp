#pragma once

#include <vector>
#include "Component.hpp"

enum Action
{
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
    Dance,
    Size
};

class InputComponent : public Component
{
    public:
        InputComponent();
        ~InputComponent();

        std::vector<bool> actions;
};