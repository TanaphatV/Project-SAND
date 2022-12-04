#pragma once
#include "Mesh.h"

class SquareMesh : public Mesh {
private:
	static SquareMesh* instance;
	SquareMesh();
public:
  virtual void draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform);
  virtual void loadData(GLRenderer* renderer);
  static SquareMesh* getInstance();
};