#include "Grid.hpp"
#include <iostream>
#include <unordered_map>
#include <set>

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

void Grid::CheckCollisions()
{
    for (int row =0; row < this->cells.size() ; row++)
    {
        for (int col = 0; col < this->cells[row].size(); col++)
        {

            this->cells[row][col]->CheckCollisions();
        }
    }
}

void Grid::Insert(std::shared_ptr<Collider> object)
{
    int col = this->GetInsertCol(object->center);
    int row = this->GetInsertRow(object->center);
    this->cells[row][col]->objects.push_back(object);
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
    std::set<std::pair<int,int>> cellCoordinates;
    std::unordered_map<int, int> rowSeenMap;
    std::unordered_map<int, int> colSeenMap;
    for (int i = 0; i < object->points.size(); i++)
    {
        int col = this->GetInsertCol(object->points[i]);
        int row = this->GetInsertRow(object->points[i]);
        rowSeenMap[row]++;
        colSeenMap[col]++;
        cellCoordinates.insert(std::make_pair(row,col));
    }
    if (cellCoordinates.size() == 3)
    {
        int row, col;
        for (auto it=rowSeenMap.begin(); it != rowSeenMap.end(); it++)
        {
            if (it->second == 1)
            {
                row = it->second;
            }
        }
        for (auto it=colSeenMap.begin(); it != colSeenMap.end(); it++)
        {
            if (it->second == 1)
            {
                col = it->second;
            }
        }
        cellCoordinates.insert(std::make_pair(row,col));
    }
    for (auto it = cellCoordinates.begin(); it != cellCoordinates.end(); it++)
    {
        this->cells[it->first][it->second]->objects.push_back(object);
    }
}

void Grid::Insert(std::shared_ptr<Sphere> object)
{
    // TODO : Implement
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
            std::cout << "dsadsadsadsa "<< object->center.x << " " <<  object->axisRadii.x << std::endl;
        }
        else
            point.x = object->center.x + object->axisRadii.x;

        if (i > 1)
            point.z = object->center.z - object->axisRadii.z;
        else
            point.z = object->center.z + object->axisRadii.z;
        std::cout << point.x << " " << point.y << " "<<point.z<<std::endl;
        pointsToCheck.push_back(point);
    }
    // insert the points
    for (int i = 0; i < pointsToCheck.size(); i++)
    {
        int row = this->GetInsertRow(pointsToCheck[i]);
        int col = this->GetInsertCol(pointsToCheck[i]);
        std::cout << "ROW AND COL " << row << "--" << col << std::endl;
        cellCoordinates.insert(std::make_pair(row,col));
    }
    std::cout << "ds " << cellCoordinates.size() << "---" << pointsToCheck.size()<< std::endl;
    for (auto it = cellCoordinates.begin(); it != cellCoordinates.end() ; it++)
    {
        this->cells[it->first][it->second]->objects.push_back(object);
    }
}