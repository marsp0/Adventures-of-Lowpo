#pragma once

#include <memory>
#include <vector>
#include "AABB.hpp"
#include "Cell.hpp"
#include "Collider.hpp"
#include "Triangle.hpp"
#include "CollisionDetector.hpp"
#include "CollisionResolver.hpp"

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
        void Update(float deltaTime);
        
        /*
        Performs a collision check on all the cells and generates contact data.
         */
        std::vector<std::shared_ptr<Collision>> CheckCollisions();
        
        /* 
        GetInsertRow returns the row of the cell that the object needs to get inserted into
         */
        int  GetInsertRow(glm::vec3 point);

        /* 
        GetInsertCol returns the column of the cell where the object should be inserted.
         */
        int  GetInsertCol(glm::vec3 point);

    private:

        int     cellsInRow;
        float   halfWidth;
        float   gridLength;
        std::vector<std::shared_ptr<RigidBody>> objects;
        std::shared_ptr<CollisionDetector> collisionDetector;
        std::shared_ptr<CollisionResolver> collisionResolver;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};