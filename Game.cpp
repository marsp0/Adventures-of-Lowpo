#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Game.hpp"
#include "Terrain.hpp"
#include "Loader.hpp"



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

void Game::InitWorld()
{
    std::string filename = "./resources/test.dae";
    // load file
    tinyxml2::XMLElement* collada = Loader::LoadFile(filename);
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometry;
    std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> geometryInstances;
    // load objects (animated and static)
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    geometry = Loader::ParseGeometry(libraryGeometries);
    for (std::unordered_map<std::string, std::shared_ptr<Geometry>>::iterator it = geometry.begin(); it != geometry.end(); it++)
    {
        std::shared_ptr<Geometry> current = it->second;
        std::vector<float> bufferData;
        for (int i = 0; current->indices.size(); i += current->stride * 3)
        {
            std::vector<int> vertexIndices{current->indices[i],
                                            current->indices[i + current->stride],
                                            current->indices[i + 2 * current->stride]};
            std::vector<int> textureIndices{current->indices[i + 2],
                                            current->indices[i + 2 + current->stride],
                                            current->indices[i + 2 + 2 * current->stride]};
            std::vector<glm::vec3> tempVertices;
            std::vector<glm::vec2> tempTextureCoords;
            for (int j = 0; j < vertexIndices.size(); j++)
            {
                int vertexIndex = vertexIndices[j] * 3;
                int textureIndex = textureIndices[j] * 2;
                float x = current->vertices[vertexIndex];
                float y = current->vertices[vertexIndex + 1];
                float z = current->vertices[vertexIndex + 2];

                float s = current->texCoords[textureIndex];
                float t = current->texCoords[textureIndex + 1];
                
                tempVertices.push_back(glm::vec3(x,y,z));
                tempTextureCoords.push_back(glm::vec2(s,t));
            }
            glm::vec3 normal = glm::cross(tempVertices[0] - tempVertices[1], tempVertices[2] - tempVertices[1]);

            for (int j = 0; j < tempVertices.size(); j++)
            {
                bufferData.push_back(tempVertices[j].x);
                bufferData.push_back(tempVertices[j].y);
                bufferData.push_back(tempVertices[j].z);

                bufferData.push_back(normal.x);
                bufferData.push_back(normal.y);
                bufferData.push_back(normal.z);

                bufferData.push_back(tempTextureCoords[j].s);
                bufferData.push_back(tempTextureCoords[j].t);
            }
        }
    }
    // for every object in the geometry hash table
    //      create mesh
    //          1. setup buffers
    //      create texture
    //      create rigid body
    //      create game object

    tinyxml2::XMLElement* libraryVisualScenes = collada->FirstChildElement("library_visual_scenes");
    geometryInstances = Loader::ParseVisualScenesStatic(libraryVisualScenes);
    // load hitboxes
    // set up the opengl buffers.
    // link hitboxes to game objects.
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