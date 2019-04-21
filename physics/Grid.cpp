#include "Grid.hpp"
#include <set>
#include <iostream>
#include <unordered_map>

Grid::Grid(float gridLength, float halfWidth) : gridLength(gridLength), cellsInRow(ceilf(gridLength / (2 * halfWidth))), halfWidth(halfWidth)
{
    this->collisionDetector = std::make_shared<CollisionDetector>();
    this->cells.resize(this->cellsInRow);
    for (int row = 0; row < this->cells.size() ; row++)
    {
        this->cells[row].resize(this->cellsInRow);
        for (int col = 0; col < this->cells[row].size(); col++)
        {
            float x = col * 2 * halfWidth + halfWidth;
            float z = row * 2 * halfWidth + halfWidth;
            this->cells[row][col] = std::make_shared<Cell>(glm::vec3(x, 0.f, z), halfWidth);
        }
    }
}

void Grid::Update(float deltaTime)
{
    // integrate
    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->Integrate(deltaTime);
    }
    // perform collision check
    std::vector<std::shared_ptr<Collision>> collisions = this->CheckCollisions();
    // resolve the contacts from the previous step
    this->collisionResolver->Solve(collisions);
    // resolve interpenetration.
}

std::vector<std::shared_ptr<Collision>> Grid::CheckCollisions()
{
    // check current and adjacent cells
    std::vector<std::shared_ptr<Collision>> collisions;
    for (int row = 0; row < this->cells.size() ; row++)
    {
        for (int col = 0; col < this->cells[row].size(); col++)
        {
            std::vector<std::shared_ptr<Collision>> temp = this->cells[row][col]->CheckCollisions(this->collisionDetector);
            collisions.insert(collisions.end(), temp.begin(), temp.end());
        }
    }
    return collisions;
}

void Grid::Insert(std::shared_ptr<Collider> object)
{
    int col = this->GetInsertCol(object->center);
    int row = this->GetInsertRow(object->center);
    this->cells[row][col]->Insert(object);
}

void Grid::Insert(std::shared_ptr<Triangle> object)
{
    // int centerCol = this->GetInsertCol(object->center);
    // int centerRow = this->GetInsertRow(object->center);

    // get cell of center
    // get cell of P1
    // get cell of P2
    // get cell of P3
    // if we have 3 different quadrants, then we surely have an angle and thus need to check 
    // if the triangle is intersecting with the 4th cell
    int col = this->GetInsertCol(object->center);
    int row = this->GetInsertRow(object->center);
    this->cells[row][col]->Insert(object);
}

void Grid::Insert(std::shared_ptr<AABB> object)
{
    std::vector<glm::vec3> pointsToCheck;
    std::set<std::pair<int,int>> cellCoordinates;
    // get bottom 4 points of the AABB
    for (int i = 0; i < 4; i++)
    {
        glm::vec3 point;
        point.y = object->center.y - object->axisRadii.y;
        if (i % 2 == 0)
        {
            point.x = object->center.x - object->axisRadii.x;
        }
        else
            point.x = object->center.x + object->axisRadii.x;

        if (i > 1)
            point.z = object->center.z - object->axisRadii.z;
        else
            point.z = object->center.z + object->axisRadii.z;
        pointsToCheck.push_back(point);
    }
    // insert the points
    for (int i = 0; i < pointsToCheck.size(); i++)
    {
        int row = this->GetInsertRow(pointsToCheck[i]);
        int col = this->GetInsertCol(pointsToCheck[i]);
        cellCoordinates.insert(std::make_pair(row,col));
    }
    for (auto it = cellCoordinates.begin(); it != cellCoordinates.end() ; it++)
    {
        this->cells[it->first][it->second]->Insert(object);
    }
}

int Grid::GetInsertCol(glm::vec3 point)
{
    // binary search for row
    int low = 0;
    int high = this->cellsInRow - 1;
    while (high > low)
    {
        int mid = (high + low) / 2;
        std::shared_ptr<Cell> midCell = this->cells[0][mid];
        float distance = abs(point.x - midCell->center.x);
        if (distance < this->halfWidth)
        {
            high = mid;
            break;
        }
        else if (point.x < midCell->center.x)
        {
            high = mid - 1;
        }
        else if (point.x > midCell->center.x)
        {
            low = mid + 1;
        }
    }
    return high;
}

int Grid::GetInsertRow(glm::vec3 point)
{
    int low = 0;
    int high = this->cellsInRow - 1;
    while (high > low)
    {
        int mid = (high + low) / 2;
        std::shared_ptr<Cell> midCell = this->cells[mid][0];
        float distance = abs(point.z - midCell->center.z);
        if (distance < this->halfWidth)
        {
            high = mid;
            break;
        }
        else if (point.z < midCell->center.z)
        {
            high = mid - 1;
        }
        else if (point.z > midCell->center.z)
        {
            low = mid + 1;
        }
    }
    return high;
}