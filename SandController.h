#pragma once
#include "BoxMesh.h"
#include <vector>
#include <bitset>

typedef glm::vec<3,int,glm::packed_highp> sandPos;

class Array3d
{
	size_t height, depth;
	vector<char> data;// need to be char cuz bool can't be lvalue lmao

public:
	Array3d(size_t width, size_t height, size_t depth) :
		height(height),
		depth(depth),
		data(width* height* depth)
	{}

	char& at(size_t x, size_t y, size_t z)
	{
		return data.at(x * height * depth + y * depth + z);
	}

	char& at(sandPos s)
	{
		return data.at(s.x * height * depth + s.y * depth + s.z);
	}

	int size()
	{
		return data.size();
	}

	void reset()
	{
		for (auto &t : data)
			t = false;
	}
};

class SandController
{
	private:
		glm::mat4 box;

		int size;
		Array3d sandGrid;
		Array3d fixedSandGrid;
		vector<glm::mat4> sandMat;//unfixed sand that are still moving
		vector<pair<glm::mat4, sandPos>> fixedSand;//fixed sand store still sands
		vector<sandPos> sandToUpdate;//used to help excluding sands that doesnt need updating
		vector<size_t> fixedToExclude;
		glm::vec3 boxScale;
		void ComputeNextPos(sandPos& sand, size_t index);
		bool ExcludeFromDraw(const sandPos sand, size_t index);
	public:
		SandController(int size, float boxScale, glm::mat4 center);
		void UpdateSandPos();
		void DrawSand(const glm::mat4& mat, GLRenderer* renderer);
		void DrawAllSand(GLRenderer* renderer);
		void AddSand(int x, int y, int z);
		void Init();
	//2D - 3D array of bool,

};