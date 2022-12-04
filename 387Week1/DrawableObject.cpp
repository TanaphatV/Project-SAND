#include "DrawableObject.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



glm::mat4 DrawableObject::getTransform()
{
  glm::mat4 transform = glm::mat4(1.0);
  transform = glm::translate(transform, glm::vec3(pos.x, pos.y, pos.z));
  transform = glm::scale(transform, glm::vec3(size.x, size.y, size.z));
  return transform;
}

DrawableObject::DrawableObject()
{
  pos = glm::vec3(0.0, 0.0, 0.0);
  size = glm::vec3(1.0, 1.0, 1.0);
}


DrawableObject::~DrawableObject()
{
}

void DrawableObject::setSize(float sizeX, float sizeY, float sizeZ )
{
  
}

void DrawableObject::setPosition(glm::vec3 newPosition)
{
  pos = newPosition;
}

void DrawableObject::translate(glm::vec3 moveDistance)
{
  pos = pos + moveDistance;
}

glm::vec3 DrawableObject::getPosition()
{
  return pos;
}


