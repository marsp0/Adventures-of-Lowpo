#pragma once

#include <memory>
#include "Scene.hpp"


enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    public:
        Game(unsigned int width, unsigned int height);
        ~Game();

        void Init();
        void Run();
        void HandleInput(float deltaTime);
        void Update(float deltaTime);
        void Render();

        // data
        GameState state;

        // Input
        bool keys[1024];
        double lastPositionX;
        double lastPositionY;

        unsigned int width, height;
        GLFWwindow* window;

        float deltaTime;
        
        std::unique_ptr<Scene> scene;
};