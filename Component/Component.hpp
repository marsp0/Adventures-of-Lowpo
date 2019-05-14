#pragma once

#include <cstdint>

enum ComponentType
{
    Transform   = (1U << 0),
    Physics     = (1U << 1),
    Rendering   = (1U << 2),
};

class Component
{
    public:

        // Constructor / Destructor
        Component(std::uint32_t type);
        ~Component();
        std::uint32_t GetComponentType();

    private:

        std::uint32_t type;
};