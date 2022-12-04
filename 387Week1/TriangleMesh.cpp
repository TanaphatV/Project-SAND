#include "TriangleMesh.h"

TriangleMesh* TriangleMesh::instance = nullptr;

void TriangleMesh::loadData(GLRenderer* renderer)
{
  //VBO data
  GLfloat vertexData[] =
  {
    0.0f,  0.0f, //triangle
    1.0f,  0.0f,
    0.5f,  1.0f  //end triangle
  };

  //GLfloat colorData[] =
  //{
  //  0.0f, 0.0f, 1.0f,
  //  0.0f, 0.0f, 1.0f,
  //  0.0f, 0.0f, 1.0f,
  //};

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);
  //Create VBO
  glGenBuffers(1, &vboId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glEnableVertexAttribArray(renderer->getVertexPosAttributeID());
  glVertexAttribPointer(renderer->getVertexPosAttributeID(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
}

TriangleMesh::TriangleMesh()
{
}


void TriangleMesh::draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform)
{
    glUniform3f(colorId, color.r, color.g, color.b);
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



TriangleMesh* TriangleMesh::getInstance()
{
    if (instance == nullptr) {
        instance = new TriangleMesh();
    }
    return instance;
}