#pragma once

#include <memory>
#include <vector>
#include "Collider.hpp"
#include "Cell.hpp"

class Grid
{
    public:
        
        Grid(float gridLength, float halfWidth);
        void Insert(std::shared_ptr<Collider> object);
        int  GetInsertRow(std::shared_ptr<Collider> object);
        int  GetInsertCol(std::shared_ptr<Collider> object);
        void Delete(std::shared_ptr<Collider> object);
        void Update(std::shared_ptr<Collider> object);
        void CheckCollisions();

    private:

        int cellsInRow;
        float halfWidth;
        float gridLength;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};