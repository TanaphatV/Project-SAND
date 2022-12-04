#include "SnowMan.h"
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

#include "CircleMesh.h"
#include "SquareMesh.h"
#include "TriangleMesh.h"

using namespace std;

void SnowMan::draw(GLuint colorId, GLuint matrixId, glm::mat4 transform, long time)
{
    glm::mat4 pos = transform;
    if (time - snowManPosTime > 200) {
        if (goLeft) {
            snowManPos -= 0.05;
            if (snowManPos < -7.5)
                goLeft = false;
        }
        else {
            snowManPos += 0.05;
            if (snowManPos > 7.5)
                goLeft = true;
        }
        snowManPosTime = time;
    }
    pos = glm::translate(pos, glm::vec3(snowManPos, 0.0f, 0.0f));

    if (time - lastTime > 500) {
        angle = -angle;
        lastTime = time;
    }
    vector <glm::mat4> matrixStack;

    glm::mat4 currentMatrix = pos;
    //
    currentMatrix = currentMatrix * glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.1f, 0.0f));
    matrixStack.push_back(currentMatrix);

    //draw body
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.6, 0.6, 0.6));
    
    CircleMesh::getInstance()->draw(colorId, glm::vec3(245.0f / 255, 245.0f / 255, 245.0f / 255), matrixId, currentMatrix);


    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);

    //Draw face/
    currentMatrix = currentMatrix * glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.45f, 0.0f));
    currentMatrix = glm::rotate(currentMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    matrixStack.push_back(currentMatrix);

    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.4, 0.4, 0.4));
    CircleMesh::getInstance()->draw(colorId, glm::vec3(245.0f / 255, 245.0f / 255, 245.0f / 255), matrixId, currentMatrix);

    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);
    //DrawLeft eye
    currentMatrix = glm::translate(currentMatrix, glm::vec3(0.10f, 0.05f, 0.0f));
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.05, 0.05, 0.05));
    CircleMesh::getInstance()->draw(colorId, glm::vec3(0.0f, 0.0f, 0.0f), matrixId, currentMatrix);

    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);
    //DrawRight eye
    currentMatrix = currentMatrix * glm::translate(glm::mat4(1.0), glm::vec3(-0.10f, 0.05f, 0.0f));
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.05, 0.05, 0.05));
    CircleMesh::getInstance()->draw(colorId, glm::vec3(0.0f, 0.0f, 0.0f), matrixId, currentMatrix);

    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);
    //Draw mouth
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.05, 0.05, 0.05));
    currentMatrix = glm::translate(currentMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
    TriangleMesh::getInstance()->draw(colorId, glm::vec3(1.0f, 20.0f / 255, 147.0f / 255), matrixId, currentMatrix);

    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);

    currentMatrix = glm::translate(currentMatrix, glm::vec3(-0.25f, 0.07f, 0.0f));
    currentMatrix = glm::rotate(currentMatrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    currentMatrix = glm::translate(currentMatrix, glm::vec3(-0.15f, 0.0f, 0.0f));
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.3f, 0.06f, 1.0f));
    SquareMesh::getInstance()->draw(colorId, glm::vec3(128.0f / 255, 90.0f / 255, 64.0f / 255), matrixId, currentMatrix);

    currentMatrix = matrixStack.back();
    matrixStack.pop_back();
    matrixStack.push_back(currentMatrix);
    currentMatrix = glm::translate(currentMatrix, glm::vec3(0.25f, 0.07f, 0.0f));
    currentMatrix = glm::rotate(currentMatrix, glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    currentMatrix = glm::translate(currentMatrix, glm::vec3(0.15f, 0.0f, 0.0f));
    currentMatrix = glm::scale(currentMatrix, glm::vec3(0.3f, 0.06f, 1.0f));
    SquareMesh::getInstance()->draw(colorId, glm::vec3(128.0f / 255, 90.0f / 255, 64.0f / 255), matrixId, currentMatrix);
}
