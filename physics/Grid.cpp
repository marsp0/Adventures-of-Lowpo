#include "Grid.hpp"
#include <iostream>

Grid::Grid(float gridLength, float halfWidth) : gridLength(gridLength), cellsInRow(ceilf(gridLength / (2 * halfWidth))), halfWidth(halfWidth)
{
    std::cout << this->cellsInRow << std::endl;

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
    std::cout << this->cells.size() << std::endl;
    std::cout << this->cells[0].size() << std::endl;

    // for (int i = 0; i < this->cellsInRow; i++)
    // {
    //     for (int j = 0; j < this->cellsInRow; j++)
    //     {
    //         float z = i * 2 * halfWidth + halfWidth;
    //         float x = j * 2 * halfWidth + halfWidth;
    //         std::shared_ptr<Cell> cell = std::make_shared<Cell>(glm::vec3(x, 0.f, z), halfWidth);
    //         this->cells.push_back(cell);
    //     }
    // }
}

void Grid::Insert(std::shared_ptr<Collider> object)
{
    
    int col = this->GetInsertCol(object);
    int row = this->GetInsertRow(object);
    this->cells[row][col]->objects.push_back(object);
}

int Grid::GetInsertCol(std::shared_ptr<Collider> object)
{
    // binary search for row
    int low = 0;
    int high = this->cellsInRow - 1;
    while (high > low)
    {
        int mid = (high + low) / 2;
        std::shared_ptr<Cell> midCell = this->cells[0][mid];
        float distance = abs(object->center.x - midCell->center.x);
        if (distance < this->halfWidth)
        {
            high = mid;
            break;
        }
        else if (object->center.x < midCell->center.x)
        {
            high = mid - 1;
        }
        else if (object->center.x > midCell->center.x)
        {
            low = mid + 1;
        }
    }
    return high;
}

int Grid::GetInsertRow(std::shared_ptr<Collider> object)
{
    int low = 0;
    int high = this->cellsInRow - 1;
    while (high > low)
    {
        int mid = (high + low) / 2;
        std::shared_ptr<Cell> midCell = this->cells[mid][0];
        float distance = abs(object->center.z - midCell->center.z);
        if (distance < this->halfWidth)
        {
            high = mid;
            break;
        }
        else if (object->center.z < midCell->center.z)
        {
            high = mid - 1;
        }
        else if (object->center.z > midCell->center.z)
        {
            low = mid + 1;
        }
    }
    return high;
}

void Grid::CheckCollisions()
{
    for (int row =0; row < this->cells.size() ; row++)
    {
        for (int col = 0; col < this->cells[row].size(); col++)
        {

            this->cells[row][col]->CheckCollisions();
            std::cout << "completed " << row << "---" << col << std::endl;
        }
    }
}