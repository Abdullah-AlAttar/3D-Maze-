#include "SceneManager.h"

SceneManager::SceneManager()
{
	gameObjects.resize(0);
}

void SceneManager::End()
{
	delete this;
}
