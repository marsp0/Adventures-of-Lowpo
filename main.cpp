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
    points.push_back(glm::vec3(1.f,0.f,0.f));
    points.push_back(glm::vec3(2.f,2.f,2.f));
    points.push_back(glm::vec3(3.f,1.f,0.f));
    std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(glm::vec3(20.f,0.f,20.f), glm::vec3(1.0f,1.f,1.f), points, ColliderType::TRIANGLE, DynamicType::STATIC);
    grid.Insert(triangle);
    std::shared_ptr<AABB> box = std::make_shared<AABB>(glm::vec3(3.f,1.f,1.f), glm::vec3(1.0f,1.f,1.f), ColliderType::BOX, DynamicType::DYNAMIC);
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