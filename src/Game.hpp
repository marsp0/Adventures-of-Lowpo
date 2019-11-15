#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Systems/Messaging/Message.hpp"
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
    InputSys,
    PhysicsSys,
    RenderingSys,
    AnimationSys,
    SystemEnd
};

class Entity;
class Game
{
    public:
        Game(int width,int height);

        void Init();
        void InitConfig();
        void InitScene(std::string filename, std::vector<std::unique_ptr<Entity>>& entities);

        void Run();
        void Update(float deltaTime);

        int CreateEntityID();
        void Subscribe(MessageType message, System system);
        void Unsubscribe(MessageType message, System system);
        void Dispatch();

        // data
        GameState state;
        int       currentID;

        int                         width;
        int                         height;
        GLFWwindow*                 window;

        // Entities
        std::vector<std::unique_ptr<Entity>> entities;

        // Player
        int                         playerID;

        // messaging
        std::vector<Message>                globalQueue;
        std::vector<std::vector<System>>    messageToSystem;
        std::vector<std::vector<Message>>   systemToMessage;

        // SYSTEMS
        InputSystem                 inputSystem;
        PhysicsSystem               physicsSystem;
        AnimationSystem             animationSystem;
        RenderingSystem             renderingSystem;
};