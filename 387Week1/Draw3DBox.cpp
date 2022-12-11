// OpenGL headers

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

void initTextVBO();
void initTexture();
GLuint TextureID = 0;
GLuint textTextureID = 0;
string textString = "Nothing";

GLuint textVAO = 0;
GLuint textVBO = 0;
GLuint textVBO2 = 0;
GLuint textVBO3 = 0;
void drawText(string s, SDL_Color color, glm::vec3 pos);
void beginDrawText();
void stopDrawText();

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
                TTF_Init();
                initTexture();
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

    int dropSize = 1;

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
                for (int i = 0; i < dropSize; i++)
                {
                    sandController.AddSand(dropPoint.x + i, sandBoxSize - 1, dropPoint.z);
                    sandController.AddSand(dropPoint.x - i, sandBoxSize - 1, dropPoint.z);
                    sandController.AddSand(dropPoint.x + i, sandBoxSize - 1, dropPoint.z + i);
                    sandController.AddSand(dropPoint.x - i, sandBoxSize - 1, dropPoint.z - i);
                    sandController.AddSand(dropPoint.x + i, sandBoxSize - 1, dropPoint.z - i);
                    sandController.AddSand(dropPoint.x - i, sandBoxSize - 1, dropPoint.z + i);
                    sandController.AddSand(dropPoint.x, sandBoxSize - 1, dropPoint.z + i);
                    sandController.AddSand(dropPoint.x, sandBoxSize - 1, dropPoint.z - i);
                    sandController.AddSand(dropPoint.x, sandBoxSize - 1, dropPoint.z);
                }
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
                case SDLK_q:
                    if(dropSize >= 2)
                        dropSize--;
                    break;
                case SDLK_e:
                        dropSize++;
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
         
            sandController.DrawAll();
            drawText(textString, {0,0,0,0}, glm::vec3(300, 570, 0));
            drawText("Sand dropper size: " + to_string(dropSize), { 0,0,0,0 }, glm::vec3(300, 540, 0));
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
            sandController.UpdateSandPos();
            timer = 0;
            SDL_GL_SwapWindow(window);
        }

        fps++;
        fpsTimer += deltaTimeS;
        timer += deltaTimeS;
        if (fpsTimer >= 1)
        {
            textString = "FPS: " + to_string( fps) + " SandCount: " + to_string(sandController.sandCount);
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

void initTexture() {
    initTextVBO();
    glActiveTexture(GL_TEXTURE0);
    //SDL_Surface* image = IMG_Load("brick.jpg");
    //if (image == NULL) {
    //    cerr << "IMG_Load: " << SDL_GetError() << endl;
    //    return;

    //}
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    //int Mode = GL_RGB;

    //if (image->format->BytesPerPixel == 4) {
    //    Mode = GL_RGBA;
    //}

    //glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);
    ////gluBuild2DMipmaps(GL_TEXTURE_2D, Mode, image->w, image->h, Mode, GL_UNSIGNED_BYTE, image->pixels);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //SDL_FreeSurface(image);

    glGenTextures(1, &textTextureID);
    glBindTexture(GL_TEXTURE_2D, textTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void initTextVBO()
{
    GLfloat vertex[] = {
        -0.5f, 0.5f,0.0f,
        -0.5f, -0.5f,0.0f,
        0.5f, 0.5f,0.0f,
        0.5f, 0.5f,0.0f,
       -0.5f, -0.5f,0.0f,
        0.5f,-0.5f,0.0f
    };

    glGenVertexArrays(1, &textVAO);
    glBindVertexArray(textVAO);
    //Create VBO
    glGenBuffers(1, &(textVBO));
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(renderer->getVertexPosAttributeID());
    glVertexAttribPointer(renderer->getVertexPosAttributeID(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glGenBuffers(1, &(textVBO2));
    glBindBuffer(GL_ARRAY_BUFFER, textVBO2);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(renderer->getVertexNormalAttributeID());
    glVertexAttribPointer(renderer->getVertexNormalAttributeID(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    GLfloat textCoord[] = {
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f
    };
    glGenBuffers(1, &(textVBO3));
    glBindBuffer(GL_ARRAY_BUFFER, textVBO3);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * 2 * sizeof(GLfloat), textCoord, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(renderer->getVertexNormalAttributeID());
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(3);
}

TTF_Font* font;
GLuint modeID;
void beginDrawText()
{

}

void stopDrawText()
{
    glUniform1i(modeID, 0);
}

void drawText(string s, SDL_Color color, glm::vec3 pos) {

    GLuint gProgramID = renderer->gProgramId;
    modeID = glGetUniformLocation(gProgramID, "mode");

    glUniform1i(modeID, 1);
    glBindTexture(GL_TEXTURE_2D, textTextureID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //textString = "RED";
    font = TTF_OpenFont("cour.ttf", 20);
    if (font == nullptr)
        cout << "NULL";

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, s.c_str(), color);
    int colors = surfaceMessage->format->BytesPerPixel;
    GLuint texture_format;
    if (colors == 4) {   // alpha
        if (surfaceMessage->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else {             // no alpha
        if (surfaceMessage->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMessage->w, surfaceMessage->h, 0, texture_format, GL_UNSIGNED_BYTE, surfaceMessage->pixels);
    int tw = surfaceMessage->w;
    int th = surfaceMessage->h;
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(font);

    GLuint mMatrix = glGetUniformLocation(gProgramID, "mMatrix");
    GLuint pMatrix = glGetUniformLocation(gProgramID, "pMatrix");
    GLuint cMatrix = glGetUniformLocation(gProgramID, "cMatrix");
    //cout << "m " << mMatrix << "p " << pMatrix << "c " << cMatrix << endl;
    glUniformMatrix4fv(cMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    renderer->resetCamera();
    glm::mat4 projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f);
    glUniformMatrix4fv(pMatrix, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), pos);


    matrix = glm::scale(matrix, glm::vec3(tw, th, 1.0f));
    glUniformMatrix4fv(mMatrix, 1, GL_FALSE, glm::value_ptr(matrix));

    //cout << "Color ID " << renderer->getColorUniformId() << endl;

    glUniform4f(renderer->getColorUniformId(), 0.0f, 0.0f, 1.0f, 1.0f);
    glBindVertexArray(textVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform1i(modeID, 0);
    //glDisable(GL_BLEND);
}





