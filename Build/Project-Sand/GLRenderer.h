#pragma once

#include <iostream>
#include <GL\glew.h>
#include "Shader.h"
#include <map>
#include <vector>
#include <string>

#include "glm.hpp"
//#include "DrawableObject.h"
//#include "MeshVbo.h"

using namespace std;

class GLRenderer
{
protected:
  int winWidth = 600;
  int winHeight =800;

  glm::mat4 projectionMatrix = glm::mat4(1.0f);
  glm::mat4 cameraMatrix = glm::mat4(1.0f);

  GLuint matrixId = -1;
  GLuint pMatrixId = -1;
  GLuint cMatrixId = -1;
  GLuint colorUniformId = -1;
  int trianglePos = 0;

  GLuint vertexAttrLocation =-1;
  GLuint normalAttrLocation = -1;

  void printProgramLog(GLuint program);
  virtual bool initialize(string vertexShaderFile, string fragmentShaderFile);
  Shader *vertexShader = nullptr;
  Shader *fragmentShader = nullptr;


  
public:
  GLRenderer(int w,int h);
  bool initGL(string vertexShaderFile, string fragmentShaderFile);

  GLuint gProgramId = -1;
  ~GLRenderer();

  void beginRender();
  void endRender();
  void resetCamera();
  void setOrthoProjection(float left, float right,float bottom, float top, float near, float far);
  void setPerspectiveProjection(float fov, float width, float height, float near, float far);
  void setCamera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
  void setViewPort(int x, int y, int w, int h);
  void resetViewPort();
  void setClearColor(float r, float g, float b);

  GLuint getModelMatrixId();
  GLuint getProjectionMatrixId();
  GLuint getCameraMatrixId();
  GLuint getColorUniformId();
  GLuint getVertexPosAttributeID();
  GLuint getVertexNormalAttributeID();
};

//class GLRendererEx2 :public GLRenderer{
//protected:
//  virtual bool initialize(string vertexShaderFile, string fragmentShaderFile);
//public:
//  GLRendererEx2();
//  virtual void render();
//};
