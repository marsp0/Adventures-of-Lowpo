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
        Deletes an object from the grid.
         */
        void Remove(std::shared_ptr<Collider>   object);
        
        /*
        Performs a collision check on all the cells and generates contact data.
         */
        std::vector<std::shared_ptr<Collision>> CheckCollisions();

        std::vector<std::shared_ptr<Collision>> CheckCells(int rowA, int colA, int rowB, int colB);

        // ===============
        // Utility methods
        // ===============

        /* 
        Returns a vector of pairs denoting the cells to be checked for collision detection
        */
        std::vector<std::pair<int, int>> GetEligibleCells(int cellRow, int cellCol);
        
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
        CollisionDetector collisionDetector;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};