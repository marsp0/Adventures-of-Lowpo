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
        Entity(int id);

        void AddComponent(std::unique_ptr<Component> component);
        // Why do i have to pass both the component name as the template arg
        // and the type as the func arg ?
        template <typename T> 
        inline T* GetComponent(std::uint32_t type)
        {
            Component* baseComponent = this->components[this->typeToIndexMap[type]].get();
            return static_cast<T*>(baseComponent);
        }
        bool HasComponent(std::uint32_t type);
        bool IsAlive();
        bool IsEligibleForSystem(std::uint32_t primaryBitset);

        int id;
        
    private:

        bool isAlive;
        std::uint32_t componentBitset;
        std::vector<std::unique_ptr<Component>> components;
        std::unordered_map<std::uint32_t, int>  typeToIndexMap;
};