#pragma once

#include "Component.hpp"

enum Movement
{
    Forward,
    Backward,
    Left,
    Right,
    Jump
};

class InputComponent : public Component
{
    public:
        InputComponent();
        ~InputComponent();

        bool actions[5];
};