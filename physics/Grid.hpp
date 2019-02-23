#pragma once

#include <memory>
#include <vector>
#include "Collider.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "BoundingBox.hpp"
#include "Cell.hpp"

class Grid
{
    public:
        
        Grid(float gridLength, float halfWidth);
        void Insert(std::shared_ptr<Collider> object);
        void Insert(std::shared_ptr<Triangle> object);
        void Insert(std::shared_ptr<Sphere> object);
        void Insert(std::shared_ptr<BoundingBox> object);
        int  GetInsertRow(glm::vec3 point);
        int  GetInsertCol(glm::vec3 point);
        void Delete(std::shared_ptr<Collider> object);
        void Update(std::shared_ptr<Collider> object);
        void CheckCollisions();

    private:

        int cellsInRow;
        float halfWidth;
        float gridLength;
        std::vector< std::vector< std::shared_ptr<Cell> > > cells;
};