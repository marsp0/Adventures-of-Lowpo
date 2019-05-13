#pragma once

#include <cstdint>

enum ComponentType
{
    Physics     = (1U << 0),
    Rendering   = (1U << 1),
};

class Component
{
    public:

        // Constructor / Destructor
        Component(std::uint32_t type);
        ~Component();

        // main methods of the component class
        virtual void start() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void end() = 0;

        std::uint32_t GetComponentType();

    private:

        std::uint32_t type;
};