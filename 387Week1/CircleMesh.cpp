#include "CircleMesh.h"
#include <math.h>
const double PI = 3.141592653589793238463;

CircleMesh* CircleMesh::instance = nullptr;

void CircleMesh::loadData(GLRenderer* renderer)
{
  //VBO data
  GLfloat vertexData[361*2];
  vertexData[0] = 0.0f;
  vertexData[1] = 0.0f;

  for (int i = 0; i < 360; i++) {
    float angle = i * (360.0f/359) ;
    vertexData[i * 2 + 2] = 0.5*cos(angle * PI/180);
    vertexData[i * 2 + 3] = 0.5*sin(angle * PI / 180);
  }

  //GLfloat colorData[361*3];
  //for (int i = 0; i < 361; i++) {
  //  colorData[i * 3] = 0.0f;
  //  colorData[i * 3 + 1] = 1.0f;
  //  colorData[i * 3 + 2] = 0.5f;

  //}

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);
  //Create VBO
  glGenBuffers(1, &vboId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER, 2 * 361 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

  glEnableVertexAttribArray(renderer->getVertexPosAttributeID());
  glVertexAttribPointer(renderer->getVertexPosAttributeID(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

}

void CircleMesh::draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform)
{
  glUniform3f(colorId, color.r, color.g, color.b);
  glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(transform));
  glBindVertexArray(vaoId);
  if (this->isFilled) {
    glDrawArrays(GL_TRIANGLE_FAN, 0, 361);
  }
  else {
    glDrawArrays(GL_LINE_LOOP, 1, 360);
  }
}

void CircleMesh::setFilled(bool fill)
{
  this->isFilled = fill;
}

CircleMesh* CircleMesh::getInstance()
{
    if (instance == nullptr) {
        instance = new CircleMesh();
    }
    return instance;
}

CircleMesh::CircleMesh() {}


