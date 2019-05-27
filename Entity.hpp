#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "Components/Component.hpp"
/* 
Entity holds a list of components that will get processed by the
various systems.
 */
class Entity
{
    public:
        Entity();

        void AddComponent(std::shared_ptr<Component> component);
        // Why do i have to pass both the component name as the template arg
        // and the type as the func arg ?
        template <typename T> 
        inline T& GetComponent(std::uint32_t type)
        {
            std::shared_ptr<Component> component = this->components[this->typeToIndexMap[type]];
            return *(static_cast<T*>(component).get());
        }
        bool HasComponent(std::uint32_t type);
        bool IsAlive();
        bool IsEligibleForSystem(std::uint32_t primaryBitset);

    private:

        bool isAlive;
        std::uint32_t componentBitset;
        std::vector<std::shared_ptr<Component>> components;
        std::unordered_map<std::uint32_t, int>  typeToIndexMap;
};