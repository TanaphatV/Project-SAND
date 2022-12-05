#pragma once
#include "GLRenderer.h"
#include <gtc/type_ptr.hpp>

class Mesh {
protected:
	GLuint vboId = 0;
	GLuint vaoId = 0;
public:
	virtual void draw(GLuint colorId, glm::vec4 color, GLuint matrixId, glm::mat4 transform) =0;
	virtual void loadData(GLRenderer* renderer) = 0;

};