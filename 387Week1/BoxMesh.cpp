#include "BoxMesh.h"
#include <vector>
#include <array>

using namespace std;

BoxMesh* BoxMesh::instance = nullptr;

void BoxMesh::draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform)
{
    glUniform3f(colorId, color.r, color.g, color.b);
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(vaoId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
glm::vec3 BoxMesh::getNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    glm::vec3 a = v2 - v1;
    glm::vec3 b = v3 - v1;
    glm::vec3 normal = glm::cross(a, b);
    normal = glm::normalize(normal);

    return normal;
}
void BoxMesh::loadData(GLRenderer* renderer)
{
    array<array<GLfloat, 3>, 8> vertex
    { {
      { {0.5f, 0.5f, 0.5f } },
      { { -0.5f,0.5f,0.5f } },
      { { -0.5f,-0.5f,0.5f } },
      { { 0.5f,-0.5f,0.5f } },
      { { 0.5f,-0.5f,-0.5f } },
      { { 0.5f,0.5f,-0.5f } },
      { { -0.5f,0.5f,-0.5f } },
      { { -0.5f,-0.5f,-0.5f } }
    } };
    array<array<GLfloat, 3>, 36> boxVertices
    { {
        //front
        vertex[0],vertex[1],vertex[2],
        vertex[0],vertex[2],vertex[3],
        //left
        vertex[1],vertex[6],vertex[7],
        vertex[1],vertex[7],vertex[2],
        //back
        vertex[4],vertex[7],vertex[5],
        vertex[7],vertex[6],vertex[5],
        //right
        vertex[5],vertex[0],vertex[3],
        vertex[5],vertex[3],vertex[4],
        //top
        vertex[5],vertex[6],vertex[1],
        vertex[5],vertex[1],vertex[0],
        //bottom
        vertex[3],vertex[2],vertex[4],
        vertex[2],vertex[7],vertex[4],

    } };
    vector <GLfloat> vertexData;
    for (int i = 0; i < 36; i++) {
        vertexData.push_back(boxVertices[i][0]);
        vertexData.push_back(boxVertices[i][1]);
        vertexData.push_back(boxVertices[i][2]);
    }

    vector <GLfloat> normalData;
    for (int i = 0; i < 6; i++) {
        glm::vec3 faceNormal = getNormal(glm::vec3(boxVertices[i * 6][0], boxVertices[i * 6][1], boxVertices[i * 6][2]),
            glm::vec3(boxVertices[i * 6 + 1][0], boxVertices[i * 6 + 1][1], boxVertices[i * 6 + 1][2]),
            glm::vec3(boxVertices[i * 6 + 2][0], boxVertices[i * 6 + 2][1], boxVertices[i * 6 + 2][2]));
        for (int j = 0; j < 6; j++) {
            normalData.push_back(faceNormal.x);
            normalData.push_back(faceNormal.y);
            normalData.push_back(faceNormal.z);
        }

    }

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    //Create VBO
    glGenBuffers(1, &(this->vboId));
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(renderer->getVertexPosAttributeID());
    glVertexAttribPointer(renderer->getVertexPosAttributeID(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glGenBuffers(1, &(this->vboId2));
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId2);
    glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(GLfloat), normalData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(renderer->getVertexNormalAttributeID());
    glVertexAttribPointer(renderer->getVertexNormalAttributeID(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    
}

BoxMesh::BoxMesh()
{
}
BoxMesh* BoxMesh::getInstance()
{
    if (instance == nullptr) {
        instance = new BoxMesh();
    }
    return instance;
}

