#include "SandController.h"

//===============================================
//TODO: REMOVE SAND THAT IS ON THE INSIDE SOMEHOW	INCREASE MARK BASED ON Y AND HOW MANY SAND LANDED ON TOP THEN ONLY CALCULATE TO NOT SHOW WHEN MARK REACHES CERTAIN NUMBER
//===============================================

SandController::SandController(int size, float boxScale, glm::mat4 center, GLRenderer* renderer) : sandGrid(size,size,size) /*, fixedSandGrid(size, size, size)*/ {
	this->size = size;
	this->boxScale = glm::vec3(boxScale,boxScale,boxScale);
	this->renderer = renderer;
	box = glm::translate(center, glm::vec3(-1, -1, -1));//start at bottom left
	SAND_COLOR = glm::vec4(246.0f / 255.0f, 215.0f / 255.0f, 176.0f / 255.0f, 0.5f);
}

void SandController::Init()
{
	sandGrid.reset();
	//fixedSandGrid.reset();
}

void SandController::Reset()
{
	sandGrid.reset();
	sandMat.clear();
	fixedSandMat.clear();
	fixedSandPos.clear();
	sandToUpdate.clear();
	sandCount = 0;
}

void SandController::UpdateSandPos()
{
	sandMat.clear();
	int upSize = sandToUpdate.size();
	int i = 0;
	for (i; i < upSize;i++)
	{
		sandPos& s = sandToUpdate.at(i);
		glm::mat4 temp = glm::scale(glm::translate(box, glm::vec3(s.x, s.y, s.z) * boxScale), boxScale);
		sandMat.push_back(temp);
		if (ComputeNextPos(s, i))
		{
			i--;
			upSize--;
			UpdateFixedSand();
		}
	}

}

void SandController::DrawSand(const glm::mat4& mat)
{
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), SAND_COLOR,
		renderer->getModelMatrixId(), mat);
}

#define BATCH_SIZE 1000
void SandController::DrawAllSand()//TODO: BATCH THE NON FIXED SANDS TOO
{
	//modelMatrices.clear();
	int sections = fixedSandMat.size() / BATCH_SIZE;
	vector<glm::mat4>::iterator it = fixedSandMat.begin();
	for (int i = 0; i < sections; i++)
	{
		int b = (BATCH_SIZE * i);
		vector<glm::mat4> temp(it + b, it + b + BATCH_SIZE);
		BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp, BATCH_SIZE);
	}

	vector<glm::mat4> temp(it + (BATCH_SIZE * sections), fixedSandMat.end());
	BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp, fixedSandMat.size() - (BATCH_SIZE * sections));


	int sections2 = sandMat.size() / BATCH_SIZE;
	vector<glm::mat4>::iterator it2 = sandMat.begin();
	for (int i = 0; i < sections2; i++)
	{
		int b = (BATCH_SIZE * i);
		vector<glm::mat4> temp(it2 + b, it2 + b + BATCH_SIZE);
		BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp, BATCH_SIZE);
	}

	vector<glm::mat4> temp2(it2 + (BATCH_SIZE * sections2), sandMat.end());
	BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp2, sandMat.size() - (BATCH_SIZE * sections2));

	//BoxMesh::getInstance()->drawInstance(renderer,renderer->getColorUniformId(), glm::vec4(246.0f / 255.0f, 215.0f / 255.0f, 176.0f / 255.0f, 0.5f), 20);
	
}

void SandController::DrawAll()
{
//	cout << dropPoint.y << endl;
	DrawAllSand();
	glm::mat4 mat = glm::translate(box, glm::vec3(1, 1, 1));
	glm::vec3 scale = boxScale * (float)size;

	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0.7, 0.7, 0.7, 1),
		renderer->getModelMatrixId(), glm::scale(glm::translate(mat,glm::vec3(0,-1 - boxScale.y,0)), glm::vec3(scale.x,boxScale.y,scale.z)));//CONTAINER BASE
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glm::mat4 drop = glm::translate(box, glm::vec3(dropPoint.x,dropPoint.y,dropPoint.z)*boxScale.x);
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0, 1, 1, 1),//DROPPOINT
		renderer->getModelMatrixId(), glm::scale(drop, boxScale * 9.0f));

	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0.8,0.8,0.8,0.2),//CONTAINER
		renderer->getModelMatrixId(), glm::scale(mat, scale));

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

//bool SandController::ExcludeFromDraw(const sandPos sand, size_t index)
//{
//	unsigned char a = 0;
//
//	a |= fixedSandGrid.at(sand.x + 1, sand.y, sand.z);
//	a |= (fixedSandGrid.at(sand.x - 1, sand.y, sand.z) << 1);
//	a |= (fixedSandGrid.at(sand.x, sand.y + 1, sand.z) << 2);
//	a |= (fixedSandGrid.at(sand.x, sand.y - 1, sand.z) << 3);
//	a |= (fixedSandGrid.at(sand.x, sand.y, sand.z + 1) << 4);
//	a |= (fixedSandGrid.at(sand.x, sand.y, sand.z - 1) << 5);
//
//	if ((a & 63) == 63)
//	{
//		fixedSandGrid.set(sand, false);
//		fixedSand.erase(fixedSand.begin() + index);
//		return true;
//	}
//
//	
//	return false;
//}

bool SandController::ComputeNextPos(sandPos& sand,size_t index)
{
	if (sand.y == 0)
	{
		glm::mat4 temp = glm::scale(glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale), boxScale);
		fixedSandMat.push_back(temp);
		fixedSandPos.push_back(sand);
		sandToUpdate.erase(sandToUpdate.begin() + index);
		//fixedSandGrid.set(sand, true);
		return true;
	}

	if (!sandGrid.at(sand.x,sand.y - (int)1, sand.z))//check bottom of sand
	{
		sandGrid.set(sand, false);
		sandGrid.set(sand.x, sand.y - (int)1, sand.z, true);
		sand = sandPos(sand.x, sand.y - (int)1, sand.z);
		return false;
	}

	for (int i = 1; i >= -1; i--)//check bottom all direction
	{
		long nX = sand.x + i;
		if (nX < 0 || nX >= size)
			continue;

		for (int j = 1; j >= -1; j--)
		{
			if (i == 0 && j == 0)
				continue;
			int nZ = sand.z + j;
			if (nZ < 0 || nZ >= size)
				continue;

			if (!sandGrid.at(nX, sand.y - (int)1, nZ))
			{
				sandGrid.set(nX, sand.y - (int)1, nZ,true);
				sandGrid.set(sand, false);
				sand = sandPos(nX, sand.y - (int)1, nZ);
				return false;
			}
		}
	}


	glm::mat4 temp = glm::scale(glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale),boxScale);
	fixedSandMat.push_back(temp);
	fixedSandPos.push_back(sand);
	sandToUpdate.erase(sandToUpdate.begin() + index);

	return true;
}

void SandController::UpdateFixedSand()
{
	if (fixedSandPos.size() == 0)
		return;

	sandPos& sand = fixedSandPos.back();
	if (sand.y == 0)
		return;
	if (sand.x == 0 || sand.x == size - 1 || sand.z == 0 || sand.z == size - 1)
		return;

	int toErase = -1;
	for (int i = 0; i < fixedSandPos.size(); i++)
	{
		if ((fixedSandPos[i].y == (sand.y - 1)) /*&& fixedSandPos[i].x == sand.x && fixedSandPos[i].z == sand.z*/) {
			if (fixedSandPos[i].x != sand.x)
				continue;
			if (fixedSandPos[i].z != sand.z)
				continue;
			toErase = i;
			erased++;

			break;
		}
	}
	if (toErase != -1)
	{
		fixedSandMat.erase(fixedSandMat.begin() + toErase);
		fixedSandPos.erase(fixedSandPos.begin() + toErase);
	}
}

void SandController::AddSand(int x,int y, int z)
{
	if (x >= size|| y >= size || z >= size || x < 0 || z < 0)
		return;

	if (!sandGrid.at(x,y,z))
	{
		sandToUpdate.push_back(sandPos(x, y, z));
		sandGrid.set(x, y, z,true);
		sandCount+= 1;
	}
}

void SandController::AddSand(sandPos s)
{
	if (s.x >= size || s.y >= size || s.z >= size || s.x < 0 || s.z < 0)
		return;
	if (!sandGrid.at(s))
	{
		sandToUpdate.push_back(s);
		sandGrid.set(s, true);
		sandCount += 1;
	}
}