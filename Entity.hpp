#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "Component/Component.hpp"
/* 
Entity holds a list of components that will get processed by the
various systems.
 */
class Entity
{
    public:
        Entity();

        void AddComponent(std::shared_ptr<Component> component);
        template <typename T> 
        inline T& GetComponent(std::uint32_t type)
        {
            std::shared_ptr<Component> component = this->components[this->typeToIndexMap[type]];
            return *(static_cast<T*>(component).get());
        }
        bool HasComponent(std::uint32_t type);
        
        bool IsAlive();
        bool IsEligibleForSystem(std::uint32_t requiredBitset);

    private:

        bool isAlive;
        std::uint32_t componentBitset;
        std::vector<std::shared_ptr<Component>> components;
        std::unordered_map<std::uint32_t, int>  typeToIndexMap;
};