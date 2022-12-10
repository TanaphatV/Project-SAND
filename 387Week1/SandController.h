#pragma once
#include "BoxMesh.h"
#include <vector>

typedef glm::vec<3,int,glm::packed_highp> sandPos;

class Array3d// packed bits into unsigned char, thus containing 8 booleans in 1 element
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
		if (x >= width || y >= height || z >= depth)
			return false;
		else if (x < 0 || y < 0 | z < 0)
			return false;

		int temp = x * height * depth + y * depth + z;
		int index = temp / 8;
		int dif = temp - (index * 8);
		return data.at(index) & (1 << dif);
	}

	bool at(sandPos s)
	{
		if (s.x >= width || s.y >= height || s.z >= depth)
			return false;
		else if (s.x < 0 || s.y < 0 | s.z < 0)
			return false;

		int temp = s.x * height * depth + s.y * depth + s.z;
		int index = temp / 8;
		int dif = temp - (index * 8);

		return data.at(index) & (1 << dif);
	}

	void set(size_t x, size_t y, size_t z, bool val)
	{
		if (x >= width || y >= height || z >= depth)
			return;
		else if (x < 0 || y < 0 | z < 0)
			return;
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
		if (s.x >= width || s.y >= height || s.z >= depth)
			return;
		else if (s.x < 0 || s.y < 0 | s.z < 0)
			return;

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

struct FixedSand{
	FixedSand(glm::vec3 m, sandPos s)
	{
		trans = m;
		pos = s;
	}
	glm::vec3 trans;
	sandPos pos;
	short mark = 0;
	bool operator==(FixedSand b)
	{
		return pos == b.pos;
	}
};

class SandController
{
	private:

		glm::mat4 box;
		GLRenderer* renderer;
		int size;
		Array3d sandGrid;
		//Array3d fixedSandGrid;
		vector<glm::mat4> sandMat;//unfixed sand that are still moving
		vector<FixedSand> fixedSand;//fixed sand store still sands
		vector<sandPos> sandToUpdate;//used to help excluding sands that doesnt need updating
		vector<size_t> fixedToExclude;
		glm::vec3 boxScale;
		void ComputeNextPos(sandPos& sand, size_t index);
		bool ExcludeFromDraw(const sandPos sand, size_t index);
		void UpdateNeighbour(const sandPos sand);
	public:
		sandPos dropPoint;
		unsigned long long sandCount = 0;
		SandController(int size, float boxScale, glm::mat4 center, GLRenderer* renderer);
		void DrawAll();
		void UpdateSandPos();
		void DrawSand(const glm::mat4& mat);
		void DrawAllSand();
		void AddSand(int x, int y, int z);
		void AddSand(sandPos s);
		void Init();
	//2D - 3D array of bool,

};