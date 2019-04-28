#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Game.hpp"
#include "Terrain.hpp"



void APIENTRY openglCallbackFunction(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           const void* userParam)
{
    using namespace std;
    cout << "---------------------opengl-callback-start------------" << endl;
    cout << "message: "<< message << endl;
    cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        cout << "OTHER";
        break;
    }
    cout << endl;
 
    cout << "id: " << id << endl;
    cout << "severity: ";
    switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
        cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        cout << "HIGH";
        break;
    }
    cout << endl;
    cout << "---------------------opengl-callback-end--------------" << endl;
}

Game::Game(int width, int height) :
    width(width), height(height)
{
    this->ambient = 0.4f;
    this->diffuse = 0.6f;
    this->Init();
    this->scene             = std::make_unique<Scene>(Scene((float)this->width, (float)this->height));
    this->renderer          = std::make_unique<Renderer>("vertex.glsl","fragment.glsl", "vertexShadow.glsl", "fragmentShadow.glsl","vertexAnimated.glsl", "fragmentAnimated.glsl", "vertexAnimatedShadow.glsl", "fragmentShadow.glsl", width, height);
    this->resourseManager   = ResourceManager();
    this->physicsEngine     = PhysicsEngine();
}

void Game::Init()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Create window
    this->window = glfwCreateWindow(this->width, this->height, "Adventures of Lowpo", nullptr, nullptr);
    glfwMakeContextCurrent(this->window);
    // Disable mouse within window
    

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    glewInit();
    glGetError();


    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unusedIds,
            true);
}

void Game::HandleInput(float deltaTime)
{
    for (int i = 0; i < this->scene->gameObjects.size() ; i++)
    {
        this->scene->gameObjects[i]->HandleInput(this->window);
    }

    // KEYBOARD
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_RELEASE)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Game::Update(float deltaTime)
{
    // TODO : Move this into the Scene class update method.
    for (int i = 0; i < this->scene->gameObjects.size() ; i++)
    {
        this->scene->gameObjects[i]->Update(deltaTime);
    }

    this->physicsEngine.Step(deltaTime, this->scene->gameObjects, this->terrain);
}

void Game::Render()
{
    this->renderer->DrawShadows(this->scene, terrain);
    this->renderer->Draw(this->scene, terrain);
}

void Game::Run()
{
    double previous = glfwGetTime();
    double lag = 0.f;
    float deltaTime = 1.f/60.f;
    while (!glfwWindowShouldClose(window))
    {
        // Game Clock
        double currentTime = glfwGetTime();
        double elapsed = currentTime - previous;
        previous = currentTime;
        lag += elapsed;
        
        glfwPollEvents();
        
        // Handle Player Input
        this->HandleInput(deltaTime);
        while (lag >= deltaTime)
        {
            // Update Game state
            this->Update(deltaTime);
            lag -= deltaTime;
        }

        // Render
        this->Render();

        glfwSwapBuffers(window);
        unsigned int error = glGetError();
        if (error != 0)
        {
            std::cout << error << std::endl;
        }
    }

    glfwTerminate();
}