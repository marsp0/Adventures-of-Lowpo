#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Systems/Physics/PhysicsSystem.hpp"
#include "Systems/Animation/AnimationSystem.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Rendering/RenderingSystem.hpp"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum System
{
    Input,
    Physics,
    Rendering,
    Animation,
};

class Entity;
class Game
{
    public:
        Game(int width,int height);

        void Init();
        void InitConfig();
        void InitScene(std::string filename, std::vector<std::shared_ptr<Entity>>& entities);

        void Run();
        void Update(float deltaTime);

        int CreateEntityID();
        void Subscribe(int event, int system);
        void Unsubscribe(int event, int system);

        // data
        GameState state;
        int       currentID;

        int                         width;
        int                         height;
        GLFWwindow*                 window;

        // Entities
        std::vector<std::shared_ptr<Entity>> entities;

        // Player
        int                         playerID;

        // messaging
        std::unordered_map<int, std::vector<int>> eventToSystemMap;

        // SYSTEMS
        InputSystem                 inputSystem;
        PhysicsSystem               physicsSystem;
        AnimationSystem             animationSystem;
        RenderingSystem             renderingSystem;
};