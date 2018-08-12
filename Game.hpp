#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include "Scene.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"


enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    public:
        Game(int width,int height);

        void Init();
        void Run();
        void HandleInput(float deltaTime);
        void Update(float deltaTime);
        void Render();

        // data
        GameState state;

        // Input
        bool                        keys[1024];
        double                      lastPositionX;
        double                      lastPositionY;

        int                         width;
        int                         height;
        GLFWwindow*                 window;

        float                       deltaTime;
        
        std::unique_ptr<Scene>      scene;
        std::unique_ptr<Renderer>   renderer;

        ResourceManager             resourseManager;
};