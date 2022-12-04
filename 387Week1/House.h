#pragma once
#include <GL\glew.h>
#include "glm.hpp"

class House
{
public:
	virtual void draw(GLuint colorId, GLuint matrixId, glm::mat4 transform, long time);

};

