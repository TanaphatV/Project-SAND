// OpenGL headers

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

#include "GLRenderer.h"
#include "BoxMesh.h"
#include "SandController.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
bool quit;

SDL_Window* window;
SDL_GLContext glContext;
SDL_Event sdlEvent;

GLRenderer* renderer = nullptr;

int main(int argc, char* argv[])
{
    quit = false;

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Initialize video subsystem
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        // Display error message
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("3DBox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            // Display error message
            cout << "Window could not be created! SDL_Error" << SDL_GetError() << endl;
            return false;
        }
        else
        {
            // Create OpenGL context
            glContext = SDL_GL_CreateContext(window);

            if (glContext == NULL)
            {
                // Display error message
                cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
                return false;
            }
            else
            {
                if (SDL_GL_SetSwapInterval(1) < 0)
                {
                    cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
                }
                renderer = new GLRenderer(800, 800);
                //Initialize OpenGL
                if (!(renderer->initGL("vertex.shader","fragment.shader")))
                {
                    cout << "Unable to initialize OpenGL! " << endl;
                    return false;
                }
                BoxMesh::getInstance()->loadData(renderer);


            }
        }
    }

    float angleH = 90;
    float angleV = 0;
    float radius = 5;
    float boxScale = 0.01;
    int sandBoxSize = 200;
    float temp = sandBoxSize / 2.0f * boxScale;
    glm::mat4 space = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.0f));
    SandController sandController(sandBoxSize,boxScale,space, renderer);
    sandController.Init();

    int fps = 0;

    float fpsTimer = 0;

    long lastFrameTime = 0;
    long deltaTime = 0;
    double deltaTimeS;
    double timer = 0;
    double SimulateFrequency = 0.0002f;
    sandController.dropPoint = sandPos(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2);
    sandPos& dropPoint = sandController.dropPoint;

    float fixedTimer = 0;
    // Game loop
    while (!quit)
    {
        long time = SDL_GetTicks();
        deltaTime = time - lastFrameTime;
        deltaTimeS = deltaTime / 1000.0f;
        lastFrameTime = time;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (fixedTimer >= 1.0f / 60.0f)//update keyboard limited to 60 second
        {
            if (keystate[SDL_SCANCODE_W])
            {
                if (dropPoint.z > 0)
                    dropPoint.z -= 1;
            }
            if (keystate[SDL_SCANCODE_S])
            {
                if (dropPoint.z < sandBoxSize - 1)
                    dropPoint.z += 1;
            }
            if (keystate[SDL_SCANCODE_A])
            {
                if (dropPoint.x > 0)
                    dropPoint.x -= 1;
            }
            if (keystate[SDL_SCANCODE_D])
            {
                if (dropPoint.x < sandBoxSize - 1)
                    dropPoint.x += 1;
            }
            if (keystate[SDL_SCANCODE_SPACE])
            {
                sandController.AddSand(dropPoint);
            }
        }
        fixedTimer += deltaTimeS;

        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            // Esc button is pressed
            if (sdlEvent.type == SDL_QUIT)
            {
                quit = true;
            }if (sdlEvent.type == SDL_KEYDOWN) {
                switch (sdlEvent.key.keysym.sym) {
                case SDLK_LEFT:
                    angleH -= 2;
                    break;
                case SDLK_RIGHT:
                    angleH += 2;
                    break;
                case SDLK_UP:
                    if (angleV < 88)
                        angleV += 2;
                    else
                        angleV = 88;
                    break;
                case SDLK_DOWN:
                    if (angleV > -90)
                        angleV -= 2;
                    else
                        angleV = -90;
                    break;
                case SDLK_EQUALS:
                    radius -= 0.5f;
                    break;
                case SDLK_MINUS:
                    radius += 0.5f;
                    break;
                }
            }
        }
       
        renderer->setClearColor(1.0f, 1.0f, 1.0f);
        renderer->beginRender();
        renderer->setPerspectiveProjection(glm::radians(45.0f), 800.f, 800.f, 1.f, 100.f);

        if (angleH > 360)
            angleH = 0;
        if (angleH < 0)
            angleH = 360;



        float x = radius * cos(glm::radians(angleV)) * cos(glm::radians(angleH));
        float z = radius * cos(glm::radians(angleV)) * sin(glm::radians(angleH));
        float y = radius * sin(glm::radians(angleV));

        renderer->setCamera(glm::vec3(x,y,z),glm::vec3(0,0,0),glm::vec3(0,1,0));

        //space = glm::rotate(space, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        //space = glm::rotate(space, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

        if (timer >= SimulateFrequency)
        {
            for (int i = 1; i < 5; i++)
            {
                sandController.AddSand(sandBoxSize / 2 + i, sandBoxSize - 1, sandBoxSize / 2);
                sandController.AddSand(sandBoxSize / 2 - i, sandBoxSize - 1, sandBoxSize / 2);
                sandController.AddSand(sandBoxSize / 2 + i, sandBoxSize - 1, sandBoxSize / 2 + i);
                sandController.AddSand(sandBoxSize / 2 - i, sandBoxSize - 1, sandBoxSize / 2 - i);
                sandController.AddSand(sandBoxSize / 2 + i, sandBoxSize - 1, sandBoxSize / 2 - i);
                sandController.AddSand(sandBoxSize / 2 - i, sandBoxSize - 1, sandBoxSize / 2 +i);
                sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2 + i);
                sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2 - i);
                sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2);
            }
         
            sandController.DrawAll();
            sandController.UpdateSandPos();
            timer = 0;
            SDL_GL_SwapWindow(window);
        }

        fps++;
        fpsTimer += deltaTimeS;
        timer += deltaTimeS;
        if (fpsTimer >= 1)
        {
            cout << "FPS: " << fps << " Time: " << time << " SandCount: " << sandController.sandCount << endl;
            fps = 0;
            fpsTimer = 0;
        }

        renderer->endRender();
    }

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    delete renderer;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}







