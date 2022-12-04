#pragma once
#include <string>
#include "Mesh.h"
using namespace std;
class CircleMesh:public Mesh
{
private:
	static CircleMesh *instance;
	CircleMesh();
protected:
  bool isFilled = true;

public:

  virtual void draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform);
  virtual void loadData(GLRenderer* renderer);
  void setFilled(bool fill);
  static CircleMesh* getInstance();
};

