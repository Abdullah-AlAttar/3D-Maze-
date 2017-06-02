#include "MazeGenerator.h"

#include <iostream>
using namespace std;
MazeGenerator::MazeGenerator(int width, int height, int seed, int fillPercent)
{
	mWidth = width;
	mHeight = height;
	mSeed = seed;
	mFillPercent = fillPercent;
	mMaze = vector<vector<int>>(mWidth, vector<int>(mHeight,0));
	visited = vector<vector<bool>>(mWidth, vector<bool>(mHeight,0));
}
#include "Time.h"
void MazeGenerator::GenerateMap()
{
	mMaze = vector<vector<int>>(mWidth, vector<int>(mHeight, 0));
	FillMap();
	for (int i = 0; i < 5; ++i)
		SmoothMap();
}

void MazeGenerator::SmoothMap()
{
	for (int i = 1; i < mWidth-1; ++i)
	{
		for (int j = 1; j < mHeight-1; j++)
		{
			int adjacentCellsCount = GetAdjacentCellsCount(i, j);
			if (adjacentCellsCount > 5  && mMaze[i][j] == 0)
			{ 
				mMaze[i][j] = 1;
				if(!StillConnected())
					mMaze[i][j] = 0;
			}
			else if (adjacentCellsCount < 1)
				mMaze[i][j] = 0;
		}
	}
}

void MazeGenerator::FillMap()
{
	srand(mSeed);
	srand(time(0));
	for (int i = 0; i < mWidth; ++i)
	{
		mMaze[i][0] = mMaze[i][mHeight - 1] = 1;
	
	}
	for (int j = 0; j < mHeight; j++)
	{
		mMaze[0][j]=mMaze[mWidth - 1][j] = 1;
	}


	mMaze[0][2] = 0;
	mStartPosition = UtilityFunctions::IndicesToWorldCoordiantes(vec2(0, 2), mWidth, mHeight);
	int idx = rand() % (mHeight-2)+1;
	mMaze[mWidth - 1][idx] = 0;
	mEndPosition = UtilityFunctions::IndicesToWorldCoordiantes(vec2(mWidth - 1, idx), mWidth, mHeight);

	for (int i = 1; i < mWidth-1; ++i)
	{
		for (int j = 1; j < mHeight-1; j++)
		{
			if (i == 0 && j == 2)continue;
			if (i == mWidth - 1 && j == idx)continue;

			if ((rand() % 100) < mFillPercent)
			{
				mMaze[i][j] = 1;
				if (!StillConnected())
					mMaze[i][j] = 0;
			}
		}
	}
}

int MazeGenerator::GetAdjacentCellsCount(int i,int j)
{
	int counter = 0;
	for (int x = i - 1; x <= i + 1; ++x)
	{
		for (int y = j - 1; y <= j + 1; ++y)
		{
			if (x >= 0 && y >= 0 && x < mWidth && y < mHeight )
			{
				if (x != i || y != j)
				{
					counter += mMaze[x][y];
				}
			}
		}
	}
	return counter;
}



vector<Path> MazeGenerator::GetAvailablePathes()
{
	vector<Path> result;

	//visited = vector<vector<bool>>(mWidth, vector<bool>(mHeight, 0));
	visitedPath = vector<vector<bool>>(mWidth, vector<bool>(mHeight, 0));

	for (int i = 0; i < 6; ++i)
	{
		vector<Path> ret = GetPathes();
		for (auto i : ret)
			result.push_back(i);
	}
	return result;
}

vector<vec3> MazeGenerator::GetEmptyPositions()
{
	vector<vec3> res;
	for (int i = 1; i < mWidth - 1; ++i)
	{
		for (int j = 1; j < mHeight - 1; ++j)
		{
			if (mMaze[i][j] == 0 )
			{
				if (visitedPath[i][j] == 0)
				{
					res.push_back(UtilityFunctions::IndicesToWorldCoordiantes(vec2(i, j), mWidth, mHeight));
				}
			}
		}
	}
	return res;
}
vector<Path> MazeGenerator::GetPathes()
{
	vector<Path> res;


	for (int i = 1; i < mWidth-1; ++i)
	{
		for (int j = 1; j < mHeight-1; ++j)
		{
			if (rand() % 2)
			{
				if (mMaze[i][j] == 0 && visitedPath[i][j] != 1)
				{
					int z = j;
					while (z < mHeight && z - j<7)
					{
						if (mMaze[i][z] != 0 || visitedPath[i][z] == 1)
							break;
						z++;
					}
					if (z - j > 3)
					{
						int x = j;
						while (x < z)
						{
							visitedPath[i][x] = 1;
							x++;
						}
						bool connected = false;
						for (int w = j; w < z; ++w)
						{
							mMaze[i][w] = 1;
							connected |= StillConnected();
							mMaze[i][w] = 0;
						}
						if(connected)
							res.push_back(Path(vec2(i, j), vec2(i, z - 1)));
					}
				}
			}
			else
			{
				if (mMaze[i][j] == 0 && visitedPath[i][j] != 1)
				{
					int z = i;
					while (z < mWidth && z - i <7)
					{
						if (mMaze[z][j] != 0 || visitedPath[z][j] == 1)
							break;
						z++;
					}
					if (z - i > 3)
					{
						int x = i;
						while (x < z)
						{
							visitedPath[x][j] = 1;
							x++;
						}
						bool connected = false;
						for (int w = i; w < z; ++w)
						{
							mMaze[w][j] = 1;
							connected |= StillConnected();
							mMaze[w][j] = 0;
						}
						if (connected)
							res.push_back(Path(vec2(i, j), vec2(z - 1, j)));
					}
				}
			}
		}
	}
	return res;
}

int MazeGenerator::GetWidth()
{
	return mWidth;
}

int MazeGenerator::GetHeight()
{
	return mHeight;
}

vec3 MazeGenerator::GetStartPosition()
{
	return mStartPosition;
}

vec3 MazeGenerator::GetFinishPosition()
{
	return mEndPosition;
}
bool MazeGenerator::StillConnected()
{
	visited = vector<vector<bool>>(mWidth, vector<bool>(mHeight, 0));
	int cnt = 0;
	for (int i = 0; i<mWidth; i++)
	{
		for (int j = 0; j<mHeight; j++)
		{
			if (mMaze[i][j] == 0 && visited[i][j] == 0)
			{
				cnt++;
				DFS(i, j);
			}
		}
	}
	return (cnt == 1);
}

void MazeGenerator::DFS(int i,int j)
{
	if (i<0 || j<0 || i == mWidth || j == mHeight)
		return;
	if (visited[i][j] == 1 || mMaze[i][j] == 1)
		return;
	visited[i][j] = 1;
	DFS(i + 1, j);
	DFS(i - 1, j);
	DFS(i, j + 1);
	DFS(i, j - 1);
}