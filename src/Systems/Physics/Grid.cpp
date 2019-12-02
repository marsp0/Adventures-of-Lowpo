#include "Grid.hpp"
#include <set>
#include <iostream>
#include <unordered_map>

Grid::Grid(float gridLength, float halfWidth) : \
            gridLength(gridLength),
            cellsInRow(ceilf(gridLength / (2 * halfWidth))), 
            halfWidth(halfWidth),
            collisionDetector(CollisionDetector()),
            cells()
{
    this->cells.resize(this->cellsInRow);
    // cell insertion
    for (int row = 0; row < this->cells.size() ; row++)
    {
        for (int col = 0; col < this->cellsInRow; col++)
        {
            float x = col * 2 * halfWidth + halfWidth;
            float z = row * 2 * halfWidth + halfWidth;
            std::unique_ptr<Cell> cell = std::make_unique<Cell>(glm::vec3(x, 0.f, z), halfWidth, row, col);
            this->cells[row].push_back(std::move(cell));
        }
    }
}

std::vector<std::shared_ptr<Collision>> Grid::CheckCollisions()
{
    // check current and adjacent cells
    std::vector<std::shared_ptr<Collision>> collisions;
    for (int row = 0; row < this->cells.size() ; row++)
    {
        for (int col = 0; col < this->cells[row].size(); col++)
        {
            std::vector<std::pair<int, int>> eligibleCells = this->GetEligibleCells(row, col);
            std::vector<std::shared_ptr<Collider>> dynamicCollidersA = this->cells[row][col]->GetDynamicColliders();
            for (int i = 0; i < eligibleCells.size(); i++)
            {
                int rowB = eligibleCells[i].first;
                int colB = eligibleCells[i].second;
                std::vector<std::shared_ptr<Collision>> newCollisions = this->CheckCells(row, col, rowB, colB);
                if (newCollisions.size() > 0)
                {
                    collisions.reserve(collisions.size() + newCollisions.size());
                    collisions.insert(collisions.end(), newCollisions.begin(), newCollisions.end());
                }
            }
        }
    }
    return collisions;
}

std::vector<std::shared_ptr<Collision>> Grid::CheckCells(int rowA, int colA, int rowB, int colB)
{
    std::vector<std::shared_ptr<Collision>> collisions;
    std::vector<std::shared_ptr<Collider>> dynamicCollidersA = this->cells[rowA][colA]->GetDynamicColliders();
    std::vector<std::shared_ptr<Collider>> dynamicCollidersB = this->cells[rowB][colB]->GetDynamicColliders();
    std::vector<std::shared_ptr<Collider>> staticCollidersA = this->cells[rowA][colA]->GetStaticColliders();
    std::vector<std::shared_ptr<Collider>> staticCollidersB = this->cells[rowB][colB]->GetStaticColliders();
    for (int i = 0; i < dynamicCollidersA.size(); i++)
    {
        // the starting point of dynamicCollidersB changes based on wether or not we are checking the same cell.
        int start = 0;
        if (rowA == rowB && colA == colB)
            start = i;
        for (int j = start; j < dynamicCollidersB.size(); j++)
        {
            if (rowA == rowB && colA == colB && i == j)
                continue;
            std::shared_ptr<Collider> first = dynamicCollidersA[i];
            std::shared_ptr<Collider> second = dynamicCollidersB[j];
            std::shared_ptr<Collision> collision = this->collisionDetector.Collide(first, second);
            if (collision != nullptr)
            {
                collisions.push_back(collision);
            }
        }
    }

    for (int i = 0; i < dynamicCollidersA.size(); i++)
    {
        for (int j = 0; j < staticCollidersB.size(); j++)
        {
            std::shared_ptr<Collider> first = dynamicCollidersA[i];
            std::shared_ptr<Collider> second = staticCollidersB[j];
            std::shared_ptr<Collision> collision = this->collisionDetector.Collide(first, second);
            if (collision != nullptr)
            {
                collisions.push_back(collision);
            }
        }
    }

    // prevents us from doulbe checking dynamic -> static when checking the same cell against itself.
    if (rowA != rowB || colA != colB)
    {
        for (int i = 0; i < staticCollidersA.size(); i++)
        {
            for (int j = 0; j < dynamicCollidersB.size(); j++)
            {
                std::shared_ptr<Collider> first = staticCollidersA[i];
                std::shared_ptr<Collider> second = dynamicCollidersB[j];
                std::shared_ptr<Collision> collision = this->collisionDetector.Collide(first, second);
                if (collision != nullptr)
                {
                    collisions.push_back(collision);
                }
            }
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

void Grid::Remove(std::shared_ptr<Collider> object)
{
    int row = object->row;
    int col = object->col;
    this->cells[row][col]->Remove(object);
}

int Grid::GetInsertCol(glm::vec3 point)
{
    // binary search for row
    int low = 0;
    int high = this->cellsInRow - 1;
    while (high > low)
    {
        int mid = (high + low) / 2;
        Cell* midCell = this->cells[0][mid].get();
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
        Cell* midCell = this->cells[mid][0].get();
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

std::vector<std::pair<int, int>> Grid::GetEligibleCells(int cellRow, int cellCol)
{
    // ---|---|
    //  x | x |
    // ---|---|
    //  C | x |
    // ---|---|
    //  x | x |
    // ---|---|
    std::vector<std::pair<int, int>> result;
    for (int row = -1; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            if (cellRow + row >= 0 && cellRow + row < this->cells.size() && cellCol + col < this->cells[0].size())
            {
                result.push_back(std::make_pair<int, int>(cellRow + row, cellCol + col));
            }
        }
    }
    return result;
}

const std::vector< std::vector< std::unique_ptr<Cell>> >& Grid::GetCells()
{
    return this->cells;
}