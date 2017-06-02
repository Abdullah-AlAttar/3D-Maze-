#pragma once
#include <iostream>
#include <vector>
#include "..\Audio Manager\AudioManager.h"
#include "..\\Maze\MazeGenerator.h"
#include "..\\Entities\MazeCube.h"
#include "..\\Entities\Player.h"
#include "..\\Entities\Obstacle.h"
#include "..\Rendering\Camera.h"
#include <glm\gtc\noise.hpp>
#include "..\\Main Engine Components\Time.h"
using namespace std;

class GameManager
{
public:
	void GenerateMaze(int width, int height, int seed, int fillPercent);
	void CreatePlayer(string modelPath, string texturePath);
	void CreateMazeCubes(string modelPath, string texturePath);
	void CreateObstacles(string modelPath, string texturePath);
	void Update();
	MazeGenerator mazeGenerator;
	List<GameObject *> mazeCubes;
	List<BoxCollider> mazeCubesColliders;
	List<pair<float, float>> mazeCubesScales;
	List<Obstacle> obstacles;
	List<PathInWorld> availablePathes;
private:
	Loader *loader;
	Player *player;
	List<Path> pathesAsIndices;
	//FixedCamera fixedCam;
	//FPSCamera fpsCam;
	//FreeCamera freeCam;
};