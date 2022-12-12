#include "GLRenderer.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
//#include "DrawableObject.h"

using namespace std;

void GLRenderer::resetCamera()
{
    cameraMatrix = glm::mat4(1.0f);
    if (cMatrixId != -1) {
        glUniformMatrix4fv(cMatrixId, 1, GL_FALSE, glm::value_ptr(this->cameraMatrix));
    }
}

GLRenderer::GLRenderer(int windowWidth, int windowHeight)
{
  this->winWidth = windowWidth;
  this->winHeight = windowHeight;
  projectionMatrix = glm::ortho(-3.f,3.f,-3.f,3.f,0.f,-20.f);
  glViewport(0, 0, winWidth, winHeight);
}

bool GLRenderer::initGL(string vertexShaderFile, string fragmentShaderFile)
{
  // Initialize glew
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK)
  {
    cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
    return false;
  }


  //Initialize OpenGL
  if (!initialize(vertexShaderFile, fragmentShaderFile))
  {
    cout << "Unable to initialize OpenGL! " << endl;
    return false;
  }
  return true;
}

void GLRenderer::beginRender()
{
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT );
    // Update window with OpenGL rendering

    glUseProgram(gProgramId);
    glViewport(0, 0, winWidth, winHeight);
}

void GLRenderer::endRender()
{
    //Unbind program
    glUseProgram(NULL);
}




void GLRenderer::printProgramLog(GLuint program)
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
      cout << infoLog << endl;
     
    }

    //Deallocate string
    delete[] infoLog;
  }
  else
  {
    printf("Name %d is not a program\n", program);
  }
}

bool GLRenderer::initialize(string vertexShaderFile, string fragmentShaderFile)
{
    //Success flag
    bool success = true;

    //Generate program
    gProgramId = glCreateProgram();
    vertexShader = new Shader(vertexShaderFile,GL_VERTEX_SHADER);
    if (!vertexShader->loadSource()) {
      return false;
    }
    fragmentShader = new Shader(fragmentShaderFile, GL_FRAGMENT_SHADER);
    if (!fragmentShader->loadSource()) {
      return false;
    }
    glAttachShader(gProgramId, vertexShader->getShaderId());
    glAttachShader(gProgramId, fragmentShader->getShaderId());


    //Link program
    glLinkProgram(gProgramId);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
      cout << "Error linking program " << gProgramId << endl;
      printProgramLog(gProgramId);
      return false;
    }

    //Get vertex attribute location
    normalAttrLocation = glGetAttribLocation(gProgramId, "vertexNormal");
    if (normalAttrLocation == -1)
    {
      cout << "vertexNormal is not a valid glsl program variable" << endl;
      return false;
    }
    vertexAttrLocation = glGetAttribLocation(gProgramId, "vertexPos");
    if (vertexAttrLocation == -1)
    {
        cout << "vertexPos is not a valid glsl program variable" << endl;
        return false;
    }

    //Set up uniform id attribute
    pMatrixId = glGetUniformLocation(gProgramId, "pMatrix");
    matrixId = glGetUniformLocation(gProgramId, "mMatrix");
    cMatrixId = glGetUniformLocation(gProgramId, "cMatrix");
    colorUniformId = glGetUniformLocation(gProgramId, "uColor");
 
    //glViewport(0, 0, this->winWidth, this->winHeight);
    //Initialize clear color
    glClearColor(1.0f, 1.0f, 1.0f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
  
}

GLRenderer::~GLRenderer()
{
    if (this->vertexShader != nullptr) {
        delete vertexShader;
    }
    if (this->fragmentShader != nullptr) {
        delete fragmentShader;
    }

}


void GLRenderer::setOrthoProjection(float left, float right, float bottom, float top,float near, float far)
{
  projectionMatrix = glm::ortho(left, right, bottom,top,near,far);
  if (pMatrixId != -1) {
      glUniformMatrix4fv(pMatrixId, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
  }
}

void GLRenderer::setPerspectiveProjection(float fov, float width, float height, float near, float far)
{
    projectionMatrix = glm::perspectiveFov( fov,  width,  height,  near,  far);
    if (pMatrixId != -1) {
        glUniformMatrix4fv(pMatrixId, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
    }
}

void GLRenderer::setCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up )
{
    cameraMatrix = glm::lookAt(eye,center, up);
    if (cMatrixId != -1) {
        glUniformMatrix4fv(cMatrixId, 1, GL_FALSE, glm::value_ptr(this->cameraMatrix));
    }
}

void GLRenderer::setViewPort(int x, int y, int w, int h)
{
  glViewport(x, y, w, h);
}

void GLRenderer::resetViewPort()
{
  glViewport(0, 0, winWidth, winHeight);
}

void GLRenderer::setClearColor(float r, float g, float b)
{
  glClearColor(r, g, b, 1.0);
}

GLuint GLRenderer::getModelMatrixId()
{
  return this->matrixId;
}

GLuint GLRenderer::getProjectionMatrixId()
{
  return this->pMatrixId;
}

GLuint GLRenderer::getCameraMatrixId()
{
    return this->cMatrixId;
}

GLuint GLRenderer::getColorUniformId()
{
  return this->colorUniformId;
}

GLuint GLRenderer::getVertexPosAttributeID()
{
    return vertexAttrLocation;
}

GLuint GLRenderer::getVertexNormalAttributeID()
{
    return this->normalAttrLocation;
}
