#pragma once
#include "BoxMesh.h"
#include <vector>

typedef glm::vec<3,int,glm::packed_highp> sandPos;

class Array3d
{
	size_t width, height, depth;
	vector<unsigned char> data;// need to be char cuz bool can't be lvalue lmao

public:
	Array3d(size_t width, size_t height, size_t depth) :
		width(width),
		height(height),
		depth(depth),
		data(((width* height* depth)/8) + 1)
	{}

	bool at(size_t x, size_t y, size_t z)
	{
		int temp = x * height * depth + y * depth + z;
		int index = temp / 8;
		int dif = temp - (index * 8);
		return data.at(index) & (1 << dif);
	}

	bool at(sandPos s)
	{
		int temp = s.x * height * depth + s.y * depth + s.z;
		int index = temp / 8;
		int dif = temp - (index * 8);

		return data.at(index) & (1 << dif);
	}

	void set(size_t x, size_t y, size_t z, bool val)
	{
		int temp = x * height * depth + y * depth + z;
		int index = temp / 8;
		int dif = temp - (index * 8);

		if(val)
			data.at(index) |= (1 << dif);
		else
			data.at(index) &= ~(1 << dif);
	}

	void set(sandPos s, bool val)
	{
		int temp = s.x * height * depth + s.y * depth + s.z;
		int index = temp / 8;
		int dif = temp - (index * 8);

		if (val)
			data.at(index) |= (1 << dif);
		else
			data.at(index) &= ~(1 << dif);
	}

	int size()
	{
		return width * height * depth;
	}

	void reset()
	{
		for (auto &t : data)
			t = 0;
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