#include "SandController.h"

//===============================================
//TODO: REMOVE SAND THAT IS ON THE INSIDE SOMEHOW
//===============================================

SandController::SandController(int size, float boxScale, glm::mat4 center) : sandGrid(size,size,size)/*, fixedSandGrid(size/2,size/2,size/2)*/{
	this->size = size;
	this->boxScale = glm::vec3(boxScale,boxScale,boxScale);
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

void SandController::DrawSand(const glm::mat4& mat, GLRenderer* renderer)
{
	BoxMesh::getInstance()->draw(renderer->getColorUniformId(), glm::vec3(0.0f, 1.0f, 1.0f),
		renderer->getModelMatrixId(), glm::scale(mat, boxScale));
}

void SandController::DrawAllSand(GLRenderer* renderer)
{

	for (int i = 0; i < fixedSand.size(); i++)
	{
		//if (ExcludeFromDraw(fixedSand.at(i).second, i))
		//{
		//	continue;
		//}
		DrawSand(fixedSand.at(i).first, renderer);
	}

	for (const glm::mat4 &sand : sandMat)
	{
		DrawSand(sand, renderer);
	}


}

//bool SandController::ExcludeFromDraw(const sandPos sand, size_t index)
//{
//
//	for (int xyz = 0; xyz < 3; xyz++)
//	{
//		for (int i = -1; i <= 1; i++)
//		{
//			int n = sand[xyz] + i;
//			if (n >= size || n <= 0)
//				return false;
//
//			switch (xyz) {
//			case 0: if (!fixedSandGrid.at(sand.x + n, sand.y, sand.z)) return false;
//			case 1: if (!fixedSandGrid.at(sand.x, sand.y + n, sand.z)) return false;
//			case 2: if (!fixedSandGrid.at(sand.x, sand.y, sand.z + n)) return false;
//			}
//		}
//		
//	}
//	
//	fixedToExclude.push_back(index);
//	return true;
//}

void SandController::ComputeNextPos(sandPos& sand,size_t index)
{
	if (sand.y == 0)
	{
		glm::mat4 temp = glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale);
		sandToUpdate.erase(sandToUpdate.begin() + index);
		fixedSand.push_back(pair<glm::mat4,sandPos>(temp,sand));
		//fixedSandGrid.at(sand) = true;
		return;
	}

	if (!sandGrid.at(sand.x,sand.y - (size_t)1, sand.z))//check bottom of sand
	{
		sandGrid.at(sand) = false;
		sandGrid.at(sand.x, sand.y - (size_t)1, sand.z) = true;
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
				sandGrid.at(nX, sand.y - (size_t)1, nZ) = true;
				sandGrid.at(sand) = false;
				sand = sandPos(nX, sand.y - (size_t)1, nZ);
				return;
			}
		}
	}

	glm::mat4 temp = glm::translate(box, glm::vec3(sand.x, sand.y, sand.z) * boxScale);
	sandToUpdate.erase(sandToUpdate.begin() + index);
	fixedSand.push_back(pair<glm::mat4, sandPos>(temp, sand));
	//fixedSandGrid.at(sand) = true;
}

void SandController::AddSand(int x,int y, int z)
{
	if (x >= size || y >= size || z >= size)
		return;
	if (!sandGrid.at(x,y,z))
	{
		sandToUpdate.push_back(sandPos(x, y, z));
		sandGrid.at(x, y, z) = true;
	}
}