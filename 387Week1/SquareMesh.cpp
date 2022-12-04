#include "SquareMesh.h"

SquareMesh* SquareMesh::instance = nullptr;
void SquareMesh::loadData(GLRenderer* renderer)
{
  //VBO data
  GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f,  0.5f,
    -0.5f,  0.5f
  };

  //GLfloat colorData[] =
  //{
  //  0.0f, 0.0f, 1.0f,
  //  0.5f, 0.0f, 0.5f,
  //  0.0f, 0.5f, 0.5f,
  //  0.0f, 0.0f, 0.5f,
  //};

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);
  //Create VBO
  glGenBuffers(1, &vboId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER,  sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glEnableVertexAttribArray(renderer->getVertexPosAttributeID());
  glVertexAttribPointer(renderer->getVertexPosAttributeID(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
}

SquareMesh::SquareMesh()
{
}

void SquareMesh::draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform)
{
    glUniform3f(colorId, color.r, color.g, color.b);
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

SquareMesh* SquareMesh::getInstance()
{
    if (instance == nullptr) {
        instance = new SquareMesh();
    }
    return instance;
}