#pragma once

#include <memory>
#include <vector>
#include "AABB.hpp"
#include "Cell.hpp"
#include "Collider.hpp"
#include "Triangle.hpp"
#include "CollisionDetector.hpp"

class Grid
{
    public:
        
        /* 
        Divides the 3d space into a 2d grid
         */
        Grid(float gridLength, float halfWidth);
        /* 
        Insert a Collider collider
         */
        void Insert(std::shared_ptr<Collider>   object);
        /* 
        Insert a Triangle collider
         */
        void Insert(std::shared_ptr<Triangle>   object);
        /* 
        Insert an AABB collider
         */
        void Insert(std::shared_ptr<AABB> object);

        /* 
        Deletes an object from the grid.
         */
        void Delete(std::shared_ptr<Collider>   object);

        /* 
        Update takes care of the collision check and the collision resolution.
         */
        void Update(std::shared_ptr<Collider>   object);
        
        /*
        Performs a collision check on all the cells and generates contact data.
         */
        void CheckCollisions();
        
        int  GetInsertRow(glm::vec3 point);
        int  GetInsertCol(glm::vec3 point);

    private:

        int     cellsInRow;
        float   halfWidth;
        float   gridLength;
        std::shared_ptr<CollisionDetector> collisionDetector;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};