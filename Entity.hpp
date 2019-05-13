#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>

class Component;
/* 
Entity holds a list of components that will get processed by the
various systems.
 */
class Entity
{
    public:
        Entity();

        void AddComponent(std::unique_ptr<Component>& component);
        bool HasComponent(std::uint32_t type);
        template <typename T> 
        T& GetComponent(std::uint32_t type); 
        
        bool IsAlive();
        bool IsEligibleForSystem(std::uint32_t requiredBitset);

    private:

        bool isAlive;
        std::uint32_t componentBitset;
        std::vector<std::unique_ptr<Component>> components;
        std::unordered_map<std::uint32_t, int>  typeToIndexMap;
};