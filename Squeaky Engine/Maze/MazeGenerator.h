#pragma once
#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include "..\Main Engine Components\UtilityFunctions.h"
using glm::vec3;
using glm::vec2;
using std::vector;
struct Path
{
	vec2 start;
	vec2 end;
	Path(vec2 _start, vec2 _end) :start(_start), end(_end) {}
};
struct PathInWorld
{
	vec3 start;
	vec3 end;
	PathInWorld(vec3 _start, vec3 _end) :start(_start), end(_end) {}
};
class MazeGenerator
{
public:
	MazeGenerator(int width, int height, int seed, int fillPercent);
	void GenerateMap();
	vector<vector<int> > mMaze;
	int GetWidth();
	int GetHeight();
	vec3 GetStartPosition();
	vec3 GetFinishPosition();
	vector<Path> GetAvailablePathes();
	vector<vec3> GetEmptyPositions();
private:
	vec3 mStartPosition;
	vec3 mEndPosition;
	vector<Path> GetPathes();
	vector<vector<bool> > visited;
	vector<vector<bool> > visitedPath;

	bool StillConnected();
	void DFS(int i,int j);
	int mWidth, mHeight, mSeed, mFillPercent;
	void SmoothMap();
	void FillMap();
	int GetAdjacentCellsCount(int i,int j);
	
};
