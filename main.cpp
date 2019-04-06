#include <iostream>

// #include "Game.hpp"
#include "physics/Grid.hpp"
#include "physics/Triangle.hpp"
#include "physics/AABB.hpp"

int main(int argc, char *argv[])
{
//     Game game(800,600);
//     // Start Game within Menu Statea
//     game.state = GAME_ACTIVE;
// //    
//     game.Run();
    Grid grid = Grid(200.f, 10.f);
    std::vector<glm::vec3> points;
    // points.push_back(glm::vec3(0.0f,0.5f,0.0f));
    points.push_back(glm::vec3(-0.07f,0.12f,0.0f));
    points.push_back(glm::vec3(2.62f,0.58f,2.0f));
    // points.push_back(glm::vec3(2.0f,0.5f,1.09f));
    points.push_back(glm::vec3(2.42,-0.32,0.5));
    std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(glm::vec3(20.0f,0.0f,20.0f), glm::vec3(1.0f,1.f,1.f), points, ColliderType::TRIANGLE, DynamicType::STATIC);
    grid.Insert(triangle);
    points.clear();
    glm::vec3 center = glm::vec3(2.3f,0.5f,.5f);
    glm::vec3 axisRadii = glm::vec3(0.5f,0.5f,0.5f);
    points.push_back(glm::vec3(center.x - axisRadii.x, center.y - axisRadii.y, center.z - axisRadii.z));
    points.push_back(glm::vec3(center.x + axisRadii.x, center.y - axisRadii.y, center.z - axisRadii.z));
    points.push_back(glm::vec3(center.x + axisRadii.x, center.y - axisRadii.y, center.z + axisRadii.z));
    points.push_back(glm::vec3(center.x - axisRadii.x, center.y - axisRadii.y, center.z + axisRadii.z));
    points.push_back(glm::vec3(center.x - axisRadii.x, center.y + axisRadii.y, center.z - axisRadii.z));
    points.push_back(glm::vec3(center.x + axisRadii.x, center.y + axisRadii.y, center.z - axisRadii.z));
    points.push_back(glm::vec3(center.x + axisRadii.x, center.y + axisRadii.y, center.z + axisRadii.z));
    points.push_back(glm::vec3(center.x - axisRadii.x, center.y + axisRadii.y, center.z + axisRadii.z));
    std::shared_ptr<AABB> box = std::make_shared<AABB>(center, axisRadii, points, ColliderType::BOX, DynamicType::DYNAMIC);
    grid.Insert(box);
    // std::shared_ptr<AABB> box1 = std::make_shared<AABB>(glm::vec3(20.f,20.f,20.f), glm::vec3(15.0f,50.f,15.f), ColliderType::BOX, DynamicType::DYNAMIC);
    // grid.Insert(box1);
    // for (int i = 0; i < 20000; i++)
    // {
    //     std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(glm::vec3(rand() % 200, rand() % 200, rand() % 200),glm::vec3(1.0f,1.f,1.f), false);
    //     grid.Insert(triangle);
    // }
    grid.CheckCollisions();
    return 0;
}