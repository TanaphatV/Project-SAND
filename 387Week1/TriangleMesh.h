#pragma once
#include "Mesh.h"
class TriangleMesh :
  public Mesh
{
private:
	static TriangleMesh* instance;
	TriangleMesh();
protected:
public:
	virtual void draw(GLuint colorId, glm::vec3 color, GLuint matrixId, glm::mat4 transform);
	virtual void loadData(GLRenderer* renderer);
	static TriangleMesh* getInstance();
};

