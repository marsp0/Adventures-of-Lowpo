#pragma once

#include <memory>
#include <vector>
#include "Collider.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "AABB.hpp"
#include "Cell.hpp"

class Grid
{
    public:
        
        Grid(float gridLength, float halfWidth);
        void Insert(std::shared_ptr<Sphere>     object);
        void Insert(std::shared_ptr<Collider>   object);
        void Insert(std::shared_ptr<Triangle>   object);
        void Insert(std::shared_ptr<AABB> object);

        void Delete(std::shared_ptr<Collider>   object);
        void Update(std::shared_ptr<Collider>   object);
        
        void CheckCollisions();
        
        int  GetInsertRow(glm::vec3 point);
        int  GetInsertCol(glm::vec3 point);

    private:

        int cellsInRow;
        float halfWidth;
        float gridLength;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};