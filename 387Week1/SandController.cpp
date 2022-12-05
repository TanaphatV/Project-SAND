#include "SandController.h"

//===============================================
//TODO: REMOVE SAND THAT IS ON THE INSIDE SOMEHOW	INCREASE MARK BASED ON Y AND HOW MANY SAND LANDED ON TOP THEN ONLY CALCULATE TO NOT SHOW WHEN MARK REACHES CERTAIN NUMBER
//===============================================

SandController::SandController(int size, float boxScale, glm::mat4 center, GLRenderer* renderer) : sandGrid(size,size,size) /*, fixedSandGrid(size, size, size)*/ {
	this->size = size;
	this->boxScale = glm::vec3(boxScale,boxScale,boxScale);
	this->renderer = renderer;
	box = glm::translate(center, glm::vec3(-1, -1, -1));//start at bottom left
}

void SandController::Init()
{
	sandGrid.reset();
	//fixedSandGrid.reset();
}

void SandController::UpdateSandPos()
{
	sandMat.clear();
	for (int i = 0; i < sandToUpdate.size();i++)
	{
		sandPos& s = sandToUpdate.at(i);
		glm::mat4 temp = glm::translate(box, glm::vec3(s.x,s.y,s.z) * boxScale);
		sandMat.push_back(temp);
		ComputeNextPos(s,i);
	}

}

void SandController::DrawSand(const glm::mat4& mat)
{
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(246.0f / 255.0f, 215.0f / 255.0f, 176.0f / 255.0f,0.5f),
		renderer->getModelMatrixId(), glm::scale(mat, boxScale));
}

void SandController::DrawAllSand()
{
	for (int i = 0; i < fixedSand.size(); i++)
	{
		DrawSand(fixedSand[i].mat);
	}

	for (const glm::mat4 &sand : sandMat)
	{
		DrawSand(sand);
	}
}

void SandController::DrawContainer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glm::mat4 mat = glm::translate(box, glm::vec3(1, 1, 1));
	
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec4(0.1,0.1,0.1,0.1),
		renderer->getModelMatrixId(), glm::scale(mat, glm::vec3(1,1,1)));

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

void SandController::ComputeNextPos(sandPos& sand,size_t index)
{
	if (sand.y == 0)
	{
		glm::mat4 temp = glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale);
		sandToUpdate.erase(sandToUpdate.begin() + index);
		fixedSand.push_back(FixedSand(temp, sand));
		//fixedSandGrid.set(sand, true);
		return;
	}

	if (!sandGrid.at(sand.x,sand.y - (size_t)1, sand.z))//check bottom of sand
	{
		sandGrid.set(sand, false);
		sandGrid.set(sand.x, sand.y - (size_t)1, sand.z, true);
		sand = sandPos(sand.x, sand.y - 1, sand.z);
		return;
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

			if (!sandGrid.at(nX, sand.y - (size_t)1, nZ))
			{
				sandGrid.set(nX, sand.y - (size_t)1, nZ,true);
				sandGrid.set(sand, false);
				sand = sandPos(nX, sand.y - (size_t)1, nZ);
				return;
			}
		}
	}

	glm::mat4 temp = glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale);
	sandToUpdate.erase(sandToUpdate.begin() + index);
	fixedSand.push_back(FixedSand(temp, sand));

	for (int i = 0; i < fixedSand.size(); i++) 
	{
		if (fixedSand[i].pos == sandPos(sand.x, sand.y - 1, sand.z)) {
			fixedSand.erase(fixedSand.begin() + i);
			break;
		}
	}
}

void SandController::AddSand(int x,int y, int z)
{
	if (x >= size || y >= size || z >= size)
		return;
	if (!sandGrid.at(x,y,z))
	{
		sandToUpdate.push_back(sandPos(x, y, z));
		sandGrid.set(x, y, z,true);
		sandCount+= 1;
	}
}