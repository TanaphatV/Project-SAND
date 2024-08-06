#include "SandController.h"

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
		if (TryMoveSand(s, i))
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


void SandController::DrawAllSand()
{
	const size_t BATCH_SIZE = 1000;
	//modelMatrices.clear();
	vector<glm::mat4> allSandMat;
	allSandMat.insert(allSandMat.end(),fixedSandMat.begin(),fixedSandMat.end());
	allSandMat.insert(allSandMat.end(), sandMat.begin(), sandMat.end());

	size_t sections = allSandMat.size() / BATCH_SIZE;
	vector<glm::mat4>::iterator it = allSandMat.begin();
	for (int i = 0; i < sections; i++)
	{
		int batchStartIt = (BATCH_SIZE * i);
		vector<glm::mat4> temp(it + batchStartIt, it + batchStartIt + BATCH_SIZE);
		BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp, BATCH_SIZE);
	}

	vector<glm::mat4> temp(it + (BATCH_SIZE * sections), allSandMat.end());
	BoxMesh::getInstance()->drawInstance(renderer, SAND_COLOR, temp, allSandMat.size() - (BATCH_SIZE * sections));
}

void SandController::DrawAll()
{
//	cout << dropPoint.y << endl;
	DrawAllSand();
	glm::mat4 mat = glm::translate(box, glm::vec3(1, 1, 1));
	glm::vec3 scale = boxScale * (float)size;

	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0, 0, 0, 1),
		renderer->getModelMatrixId(), glm::scale(glm::translate(mat,glm::vec3(0,-1 - boxScale.y,0)), glm::vec3(scale.x,boxScale.y,scale.z)));//CONTAINER BASE
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glm::mat4 drop = glm::translate(box, glm::vec3(dropPoint.x,dropPoint.y,dropPoint.z)*boxScale.x);
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0, 1, 1, 1),//DROPPOINT
		renderer->getModelMatrixId(), glm::scale(drop, boxScale * 9.0f));

	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0.8,0.8,0.8,0.2),//CONTAINER
		renderer->getModelMatrixId(), glm::scale(mat, scale));


}


bool SandController::TryMoveSand(sandPos& sand,size_t index)
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

	if (!sandGrid.ContainSandAt(sand.x,sand.y - 1, sand.z))//check bottom of sand
	{
		sandGrid.SetSand(sand, false);
		sandGrid.SetSand(sand.x, sand.y - 1, sand.z, true);
		sand = sandPos(sand.x, sand.y - 1, sand.z);
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

			if (!sandGrid.ContainSandAt(nX, sand.y - 1, nZ))
			{
				sandGrid.SetSand(nX, sand.y - 1, nZ,true);
				sandGrid.SetSand(sand, false);
				sand = sandPos(nX, sand.y - 1, nZ);
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

	int lastIndex = fixedSandPos.size() - 1;
	sandPos& lastSand = fixedSandPos.at(lastIndex);
	if (lastSand.y == 0)
		return;
	if (lastSand.x == 0 || lastSand.x == size - 1 || lastSand.z == 0 || lastSand.z == size - 1)
		return;

	int indexToErase = -1;
	for (int i = 0; i < fixedSandPos.size(); i++)//find the sand beneath the last sand
	{
		if ((fixedSandPos[i].y == (lastSand.y - 1))) {
			if (fixedSandPos[i].x != lastSand.x)
				continue;
			if (fixedSandPos[i].z != lastSand.z)
				continue;
			indexToErase = i;
			erased++;

			break;
		}
	}
	if (indexToErase != -1)//erase covered sand
	{
		fixedSandMat.erase(fixedSandMat.begin() + indexToErase);
		fixedSandPos.erase(fixedSandPos.begin() + indexToErase);
	}
}

void SandController::AddSand(int x,int y, int z)
{
	if (x >= size|| y >= size || z >= size || x < 0 || z < 0)
		return;

	if (!sandGrid.ContainSandAt(x,y,z))
	{
		sandToUpdate.push_back(sandPos(x, y, z));
		sandGrid.SetSand(x, y, z,true);
		sandCount+= 1;
	}
}

void SandController::AddSand(sandPos s)
{
	if (s.x >= size || s.y >= size || s.z >= size || s.x < 0 || s.z < 0)
		return;
	if (!sandGrid.ContainSandAt(s))
	{
		sandToUpdate.push_back(s);
		sandGrid.SetSand(s, true);
		sandCount += 1;
	}
}