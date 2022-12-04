#include "House.h"
#include "SnowMan.h"
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

#include "CircleMesh.h"
#include "SquareMesh.h"
#include "TriangleMesh.h"

void House::draw(GLuint colorId, GLuint matrixId, glm::mat4 transform, long time)
{
    glm::mat4 pos = glm::translate(transform, glm::vec3(0, 0.5, 0.0));
    SquareMesh::getInstance()->draw(colorId, glm::vec3(0.0f, 0.0f, 1.0f), matrixId, pos);
    pos = glm::translate(transform, glm::vec3(-0.5, 1, 0.0));
    TriangleMesh::getInstance()->draw(colorId, glm::vec3(1.0f, 0.0f, 0.0f), matrixId, pos);
}
