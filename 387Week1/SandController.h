#pragma once
#include "BoxMesh.h"
#include <vector>
#include <bitset>
#define WIDTH 100
#define HEIGHT 100
#define DEPTH 100

typedef glm::vec<3,int,glm::packed_highp> sandPos;

class Array3d
{
	size_t height, depth;
	bitset<WIDTH * HEIGHT * DEPTH> data;// need to be char cuz bool can't be lvalue lmao

public:
	Array3d(){}

	bool at(size_t x, size_t y, size_t z)
	{
		return data[x * height * depth + y * depth + z];
	}

	bool at(sandPos s)
	{
		return data[s.x * height * depth + s.y * depth + s.z];
	}

	void set(sandPos s,bool val)
	{
		data[s.x * height * depth + s.y * depth + s.z] = val;
	}

	void set(size_t x, size_t y, size_t z, bool val)
	{
		data[x * height * depth + y * depth + z] = val;
	}

	int size()
	{
		return data.size();
	}

	void reset()
	{
		data.reset();
	}
};

class SandController
{
	private:
		glm::mat4 box;

		int size;
		Array3d sandGrid;
		//Array3d fixedSandGrid;
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