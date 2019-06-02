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
        Entity(int entityID);

        void AddComponent(std::shared_ptr<Component> component);
        // Why do i have to pass both the component name as the template arg
        // and the type as the func arg ?
        template <typename T> 
        inline T& GetComponent(std::uint32_t type)
        {
            std::shared_ptr<Component> baseComponent = this->components[this->typeToIndexMap[type]];
            std::shared_ptr<T> component = std::static_pointer_cast<T>(baseComponent);
            return *(component.get());
        }
        bool HasComponent(std::uint32_t type);
        bool IsAlive();
        bool IsEligibleForSystem(std::uint32_t primaryBitset);

        int entityID;
        
    private:

        bool isAlive;
        std::uint32_t componentBitset;
        std::vector<std::shared_ptr<Component>> components;
        std::unordered_map<std::uint32_t, int>  typeToIndexMap;
};