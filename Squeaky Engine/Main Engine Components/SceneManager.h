#pragma once


#include "..\Entities\GameObject.h"
#include "..\Lighting\Lighting.h"
#include <iostream>
#include <vector>
#include <map>
using std::vector;
#define List vector
class SceneManager
{
public:
	SceneManager();
	 List<GameObject*> gameObjects;
	 void End();
};