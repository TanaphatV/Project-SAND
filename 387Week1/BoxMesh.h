#pragma once
#include "Mesh.h"
#include <GL\glew.h>
class BoxMesh :
  public Mesh
{
private:
	static BoxMesh* instance;
	GLuint vboId2 = 0;
	BoxMesh();
    glm::vec3 getNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
public:
	virtual void draw(GLuint colorId, glm::vec4 color, GLuint matrixId, glm::mat4 transform);
	void drawInstance(GLRenderer* renderer, glm::vec4 color, vector<glm::mat4>& m, int amount);
	virtual void loadData(GLRenderer* renderer);
	static BoxMesh* getInstance();
	GLuint vboI = 0;
	GLuint getVao() {
		return vaoId;
	}
};

