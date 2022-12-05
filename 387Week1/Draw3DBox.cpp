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

    float angleY = 0;
    float angleX = 0;
    float boxScale = 0.01;
    int sandBoxSize = 200;
    float temp = sandBoxSize / 2.0f * boxScale;
    glm::mat4 space = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5.0f));
    SandController sandController(sandBoxSize,boxScale,space, renderer);
    sandController.Init();

    int fps = 0;

    float fpsTimer = 0;

    long lastFrameTime = 0;
    long deltaTime = 0;
    float deltaTimeS;
    float timer = 0;
    float SimulateFrequency = 0.0001f;
    // Game loop
    while (!quit)
    {
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            // Esc button is pressed
            if (sdlEvent.type == SDL_QUIT)
            {
                quit = true;
            }if (sdlEvent.type == SDL_KEYDOWN) {
                switch (sdlEvent.key.keysym.sym) {
                case SDLK_LEFT:
                    angleY -= 2;
                    break;
                case SDLK_RIGHT:
                    angleY += 2;
                    break;
                case SDLK_UP:
                    angleX -= 2;
                    break;
                case SDLK_DOWN:
                    angleX += 2;
                    break;
                case SDLK_SPACE:
                    sandController.AddSand(sandBoxSize / 2 + 1, sandBoxSize - 1, sandBoxSize / 2);
                    sandController.AddSand(sandBoxSize / 2 + 1, sandBoxSize - 1, sandBoxSize / 2 + 1);
                    sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2 + 1);
                    sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2);
                }
            }
        }
        
        long time = SDL_GetTicks();
        deltaTime = time - lastFrameTime;
        deltaTimeS = deltaTime / 1000.0f;
        lastFrameTime = time;

        timer += deltaTimeS;
        renderer->setClearColor(1.0f, 1.0f, 1.0f);
        renderer->beginRender();
        renderer->setPerspectiveProjection(glm::radians(45.0f), 800.f, 800.f, 1.f, 100.f);

        space = glm::rotate(space, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        space = glm::rotate(space, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

        if (timer >= SimulateFrequency)
        {
            sandController.DrawAllSand();
            sandController.AddSand(sandBoxSize / 2 + 1, sandBoxSize - 1, sandBoxSize / 2);
            sandController.AddSand(sandBoxSize / 2 + 1, sandBoxSize - 1, sandBoxSize / 2 + 1);
            sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2 + 1);
            sandController.AddSand(sandBoxSize / 2, sandBoxSize - 1, sandBoxSize / 2);
            sandController.UpdateSandPos();
            sandController.DrawContainer();
            timer = 0;
            SDL_GL_SwapWindow(window);
        }

        fps++;
        fpsTimer += deltaTimeS;
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







