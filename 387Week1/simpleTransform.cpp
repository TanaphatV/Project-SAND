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

#include <iostream>
#include <string>
#include <vector>

using namespace std;
bool quit;

SDL_Window* window;
SDL_GLContext glContext;
SDL_Event sdlEvent;

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);
bool initGL();

//Graphics program 
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLint gUColorLocation = -1;
GLint gMMatrix = -1;
GLuint gVBO = 0;
GLuint VAO = 0;

void drawSquare(glm::vec3 color, glm::mat4 transform);

int main(int argc, char* argv[])
{
    quit = false;

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Initialize video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // Display error message
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
                // Initialize glew
                GLenum glewError = glewInit();
                if (glewError != GLEW_OK)
                {
                    cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
                    return false;
                }
                if (SDL_GL_SetSwapInterval(1) < 0)
                {
                    cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
                }

                //Initialize OpenGL
                if (!initGL())
                {
                    cout << "Unable to initialize OpenGL! " << endl;
                    return false;
                }

            }
        }
    }

    GLuint pMatrixId = glGetUniformLocation(gProgramID, "pMatrix");
    

    // Game loop
    while (!quit)
    {
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            // Esc button is pressed
            if (sdlEvent.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Set background color as cornflower blue
        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Update window with OpenGL rendering
        glUseProgram(gProgramID);
        glBindVertexArray(VAO);

        glm::mat4 projectionMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f);

        glUniformMatrix4fv(pMatrixId, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glViewport(0, 0, 800, 800);
        glm::mat4 sPos = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, 0, 0));
        drawSquare(glm::vec3(0.0f, 1.0f, 0.0f),sPos);

        sPos = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1));
        drawSquare(glm::vec3(1.0f, 1.0f, 0.0f), sPos);

        sPos = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, 0, 0));
        sPos = glm::rotate(sPos, glm::radians(45.0f), glm::vec3(0, 0, 1));

        drawSquare(glm::vec3(0.0f, 1.0f, 0.0f), sPos);

        //Disable vertex position
        //glDisableVertexAttribArray(gVertexPos2DLocation);
        //Unbind program
        glUseProgram(NULL);
        SDL_GL_SwapWindow(window);
    }

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

void drawSquare(glm::vec3 color, glm::mat4 transform) {
    glUniform3f(gUColorLocation, color.r, color.g, color.b);
    glUniformMatrix4fv(gMMatrix, 1, GL_FALSE, glm::value_ptr(transform));
    //draw square
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool initGL()
{
    //Success flag
    bool success = true;

    //Generate program
    gProgramID = glCreateProgram();

    //Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Get vertex source
    const GLchar* vertexShaderSource[] =
    {
      "#version 330\n uniform mat4 pMatrix = mat4(1.0); uniform mat4 mMatrix = mat4(1.0); in vec2 LVertexPos2D; uniform vec3 uColor; out vec3 color; void main() { gl_Position = pMatrix * mMatrix *vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); color = uColor;}"
    };

    //Set vertex source
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

    //Compile vertex source
    glCompileShader(vertexShader);

    //Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        printShaderLog(vertexShader);
        success = false;
    }
    else
    {
        //Attach vertex shader to program
        glAttachShader(gProgramID, vertexShader);


        //Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        //Get fragment source
        const GLchar* fragmentShaderSource[] =
        {
          "#version 330\n in vec3 color; out vec4 LFragment; void main() { LFragment = vec4( color, 1.0 ); }"
        };

        //Set fragment source
        glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

        //Compile fragment source
        glCompileShader(fragmentShader);

        //Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE)
        {
            printf("Unable to compile fragment shader %d!\n", fragmentShader);
            printShaderLog(fragmentShader);
            success = false;
        }
        else
        {
            //Attach fragment shader to program
            glAttachShader(gProgramID, fragmentShader);


            //Link program
            glLinkProgram(gProgramID);

            //Check for errors
            GLint programSuccess = GL_TRUE;
            glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
            if (programSuccess != GL_TRUE)
            {
                printf("Error linking program %d!\n", gProgramID);
                printProgramLog(gProgramID);
                success = false;
            }
            else
            {
                //Get vertex attribute location
                gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
                gUColorLocation = glGetUniformLocation(gProgramID, "uColor");
                gMMatrix = glGetUniformLocation(gProgramID, "mMatrix");

                if (gVertexPos2DLocation == -1)
                {
                    printf("LVertexPos2D is not a valid glsl program variable!\n");
                    success = false;
                }else {
                    //Initialize clear color
                    glClearColor(0.f, 0.f, 0.f, 1.f);

                    //VBO data
                    GLfloat data[] =
                    {
                      -0.5f, -0.5f, //square
                       0.5f, -0.5f,  
                      -0.5f,  0.5f,  
                       0.5f,  0.5f, //end square
                       0.0f,  0.0f, //triangle
                       1.0f,  0.0f, 
                       0.5f,  1.0f  //end triangle
                    };
                    vector <GLfloat> vData;
                    for (int i = 0; i < 14; i++) {
                        vData.push_back(data[i]);
                    }
                    vData.push_back(0);
                    vData.push_back(0);
                    for (int i = 0; i <= 360; i++) {
                        vData.push_back(0.5 * cos(glm::radians((float)i)));
                        vData.push_back(0.5 * sin(glm::radians((float)i)));
                    }

                    glGenVertexArrays(1, &VAO);
                    glBindVertexArray(VAO);
                    //Create VBO
                    glGenBuffers(1, &gVBO);
                    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
                    glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(GLfloat), vData.data(), GL_STATIC_DRAW);

                    glEnableVertexAttribArray(gVertexPos2DLocation);
                    glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

                }
            }
        }
    }

    return success;
}
void printProgramLog(GLuint program)
{
    //Make sure name is shader
    if (glIsProgram(program))
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf("Name %d is not a program\n", program);
    }
}

void printShaderLog(GLuint shader)
{
    //Make sure name is shader
    if (glIsShader(shader))
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf("Name %d is not a shader\n", shader);
    }
}