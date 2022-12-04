// OpenGL headers

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

// SDL headers
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_opengl.h>

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
GLuint gVBO = 0;
GLuint VAO = 0;
GLuint VAO2 = 0;
GLuint VAO3 = 0;
GLuint VAO4 = 0;

const float PI = 3.1415926f;
const int vertex_size = 100;

int main(int argc, char *argv[])
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
    window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
    //glClearColor(0.39f, 0.58f, 0.93f, 1.f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Update window with OpenGL rendering
    glUseProgram(gProgramID);
	glBindVertexArray(VAO);

    //glEnableVertexAttribArray(gVertexPos2DLocation);
    //glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    //glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glDrawArrays(GL_TRIANGLES, 0, 9);

    glBindVertexArray(VAO2);
    glDrawArrays(GL_LINE_STRIP, 0, 7);


    glBindVertexArray(VAO3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_size);

    glBindVertexArray(VAO4);
    glDrawArrays(GL_LINE_LOOP, 0, vertex_size - 1);

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
    "#version 330\n in vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
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
      "#version 330\n out vec4 LFragment; void main() { LFragment = vec4( 1.0, 0.0, 0.0, 1.0 ); }"
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
        if (gVertexPos2DLocation == -1)
        {
          printf("LVertexPos2D is not a valid glsl program variable!\n");
          success = false;
        }
        else
        {
          //Initialize clear color
          glClearColor(0.f, 0.f, 0.f, 1.f);

          //VBO data
          GLfloat vertexData[] =
          {
            -0.5f, 0.25f,
            -0.75f, 0.25f,
            -0.5f, 0.5f,
            
            -0.5f, 0.5f,
            -0.75f, 0.25f,
            -0.75f, 0.5f,

            - 0.625f, 0.65f,
            -0.5f, 0.5f,
            -0.75f, 0.5f
          };

          GLfloat vertexData2[] =
          {
              0.0f, 0.5f,
              0.0f, 0.25f,
              0.25f, 0.25f,
              0.25f, 0.5f,
              0.0f, 0.5f,            
              0.125f, 0.65f,
              0.25f, 0.5f 
          };

          
          GLfloat radius = 0.1f;
          float originX = -0.4f; float originY = 0.6f;
          GLfloat vertexData3[vertex_size * 2];
          int vertex_size_index = 0;
 
          for (int i = 0; i < vertex_size; i++)
          {
              float angle = 2.0f * PI * float(i) / float(vertex_size);
              float disX = radius * cosf(angle);
              float disY = radius * sinf(angle);
              vertexData3[vertex_size_index] = disX + originX; vertex_size_index++;
              vertexData3[vertex_size_index] = disY + originY; vertex_size_index++;
          }

          GLfloat vertexData4[vertex_size * 2];
          originX += 0.8f; vertex_size_index = 0;
          for (int i = 1; i < vertex_size; i++)
          {
              float angle = 2.0f * PI * float(i) / float(vertex_size);
              float disX = radius * cosf(angle);
              float disY = radius * sinf(angle);
              vertexData4[vertex_size_index] = disX + originX; vertex_size_index++;
              vertexData4[vertex_size_index] = disY + originY; vertex_size_index++;
          }
          

              
		  glGenVertexArrays(1, &VAO);
		  glBindVertexArray(VAO);
          //Create VBO
          glGenBuffers(1, &gVBO);
          glBindBuffer(GL_ARRAY_BUFFER, gVBO);
          glBufferData(GL_ARRAY_BUFFER, 2 * 9 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
		  glEnableVertexAttribArray(gVertexPos2DLocation);
		  glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

          glGenVertexArrays(1, &VAO2);
          glBindVertexArray(VAO2);
          //Create VBO
          glGenBuffers(1, &gVBO);
          glBindBuffer(GL_ARRAY_BUFFER, gVBO);
          glBufferData(GL_ARRAY_BUFFER, 2 * 7 * sizeof(GLfloat), vertexData2, GL_STATIC_DRAW);
          glEnableVertexAttribArray(gVertexPos2DLocation);
          glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

          glGenVertexArrays(1, &VAO3);
          glBindVertexArray(VAO3);
          //Create VBO
          glGenBuffers(1, &gVBO);
          glBindBuffer(GL_ARRAY_BUFFER, gVBO);
          glBufferData(GL_ARRAY_BUFFER, 2 * 100 * sizeof(GLfloat), vertexData3, GL_STATIC_DRAW);
          glEnableVertexAttribArray(gVertexPos2DLocation);
          glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

          glGenVertexArrays(1, &VAO4);
          glBindVertexArray(VAO4);
          //Create VBO
          glGenBuffers(1, &gVBO);
          glBindBuffer(GL_ARRAY_BUFFER, gVBO);
          glBufferData(GL_ARRAY_BUFFER, 2 * 100 * sizeof(GLfloat), vertexData4, GL_STATIC_DRAW);
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