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
    this->Init();
    this->deltaTime         = 0.016f;
    this->scene             = std::make_unique<Scene>(Scene((float)this->width, (float)this->height));
    this->renderer          = std::make_unique<Renderer>(Renderer("vertexShader.glsl","fragmentShader.glsl"));
    this->resourseManager   = ResourceManager();
    // this->physicsEngine     = PhysicsEngine();
    
    this->terrain = this->resourseManager.LoadTerrain("/home/martin/Downloads/heightmap.jpg");
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
    this->window = glfwCreateWindow(this->width, this->height, "Shootemup", nullptr, nullptr);
    glfwMakeContextCurrent(this->window);
    // Disable mouse within window
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    glewInit();
    glGetError();
    glViewport(0, 0, this->width, this->height);
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

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
    // KEYBOARD
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,FORWARD);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,BACKWARD);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,LEFT);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,RIGHT);

    // MOUSE
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);
    float xoffset = xpos - this->lastPositionX;
    float yoffset = this->lastPositionY - ypos;

    this->lastPositionX = xpos;
    this->lastPositionY = ypos;

    this->scene->camera.HandleMouseInput(deltaTime,xoffset,yoffset);

}

void Game::Update(float deltaTime)
{
    // this->physicsEngine.Step(deltaTime, this->scene->gameObjects);
    // this->physicsEngine.HandleCollisions(this->scene->gameObjects);
    std::cout << this->terrain->GetHeight(this->scene->camera.position.x, this->scene->camera.position.z) << std::endl;
}

void Game::Render()
{
    this->renderer->Draw(this->scene, terrain);
}

void Game::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Handle Player Input
        this->HandleInput(deltaTime);
        // Update Game state
        this->Update(deltaTime);
        // Render
        this->Render();
        glfwSwapBuffers(window);
        // unsigned int error = glGetError();
        // if (error != 0)
        // {
        //     std::cout << error << std::endl;
        // }

    }

    glfwTerminate();
}