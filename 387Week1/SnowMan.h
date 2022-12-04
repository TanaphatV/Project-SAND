#pragma once
#include <GL\glew.h>
#include "glm.hpp"

class SnowMan
{
	float angle = 25.0f;
	long lastTime = 0;
	float snowManPos = 0;
	long snowManPosTime = 0;
	bool goLeft = true;
public:
	virtual void draw(GLuint colorId, GLuint matrixId, glm::mat4 transform, long time);
};

