#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Collider.hpp"
#include "CollisionDetector.hpp"

/*
    Cell containts its boundries and 2 vectors of dynamic and static objects.
*/
class Cell
{
    public:
        /* 
            center - the center position of the cell.
            halfwidth - how much are we extending in all directions. 
            Currently this is a float so we are dealing with cubes, but it can be modified
            to vector to handle random sized rectangles.
        */
        Cell(glm::vec3 center, float halfWidth);

        /* 
        Inserts a collider shared pointer into the cell. Could be in dynamicObjects or staticObjects 
         */
        void Insert(std::shared_ptr<Collider> object);

        /* 
        Removes an object from the cell 
        */
        void Remove(std::shared_ptr<Collider> object);

        const std::vector<std::shared_ptr<Collider>>& GetDynamicColliders();
        const std::vector<std::shared_ptr<Collider>>& GetStaticColliders();

        // Data
        /* 
        Contains all the dynamic objects in the cell. 
        Dynamic object is any objects on which the physics will be active.
         */
        std::vector<std::shared_ptr<Collider>> dynamicColliders;

        /* 
        Contains all the environment objects. All objects here will be treated as immovable 
        and thus the physics calculations on these objects will be easier.
         */
        std::vector<std::shared_ptr<Collider>> staticColliders;
        glm::vec3 center;
        float halfWidth;
    private:

};