#include "GameManager.h"




void GameManager::GenerateMaze(int width, int height, int seed, int fillPercent)
{
	 mazeGenerator= MazeGenerator(width, height, seed, fillPercent);
	 mazeGenerator.GenerateMap();
}

void GameManager::CreatePlayer(string modelPath, string texturePath)
{
	player = new Player(modelPath, texturePath);
}

void GameManager::CreateMazeCubes(string modelPath, string texturePath)
{
	loader = new Loader();
	RawModel boxModel = loader->LoadToVAO(ModelMesh::LoadModel(modelPath));
	Texture boxTexture(loader->TextureFromFile(texturePath));
	TexturedModel boxTexturedModel(boxModel, boxTexture);
	GameObject *tmpBox;
	int nameCnt = 0;
	for (int i = 0; i < mazeGenerator.GetWidth(); ++i)
	{
		for (int j = 0; j < mazeGenerator.GetHeight(); ++j)
		{
			if (mazeGenerator.mMaze[i][j] == 1)
			{
				tmpBox = new GameObject(boxTexturedModel,
					Transform(vec3(-mazeGenerator.GetWidth() / 2 + i * 2, 1, mazeGenerator.GetHeight() / 2 + j * 2),
						vec3(0, 0, 0), glm::vec3(1, 6 * (glm::perlin(glm::vec2((i + 1) / 16.0f, (j + 1) / 16.0f)) + 1),
							1)));
				tmpBox->name = "box" + to_string(nameCnt++);
				//sceneManager->gameObjects.push_back(tmpBox);
				mazeCubesScales.push_back({ 1.0f, tmpBox->transform.scale.y });
				mazeCubes.push_back(tmpBox);
				mazeCubesColliders.push_back(BoxCollider(tmpBox->transform.position - VECTOR_ONE,
					tmpBox->transform.position + VECTOR_ONE,
					tmpBox->name));
			}
		}
	}

}

void GameManager::CreateObstacles(string modelPath, string texturePath)
{
	RawModel rawModel = loader->LoadToVAO(ModelMesh::LoadModel(modelPath));
	Texture texture(loader->TextureFromFile(texturePath));
	TexturedModel texModel(rawModel, texture);
	pathesAsIndices = mazeGenerator.GetAvailablePathes();
	for (auto i : pathesAsIndices)
		availablePathes.push_back(PathInWorld(
			UtilityFunctions::IndicesToWorldCoordiantes(i.start, mazeGenerator.GetWidth(), mazeGenerator.GetHeight()),
			UtilityFunctions::IndicesToWorldCoordiantes(i.end, mazeGenerator.GetWidth(), mazeGenerator.GetHeight())));
	for (int i = 0; i < availablePathes.size(); ++i)
	{
		obstacles.push_back(Obstacle(texModel,
			Transform(availablePathes[i].start, VECTOR_ZERO, VECTOR_ONE),
			(availablePathes[i].start.z - availablePathes[i].end.z) == 0));
	}
}

void GameManager::Update()
{
	if (Input::GetKey(GLFW_KEY_UP))
	{
		player->gameObject->transform.Translate(vec3(0, 0, +Time::GetDeltaTime()*player->movementSpeed));
		player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * 200, 0, 0));
		player->UpdateCollider();
		for (BoxCollider boxCollider : mazeCubesColliders)
		{
			HitInfo hit = player->IsColliding(boxCollider);
			if (hit.isColliding == true)
			{
				player->gameObject->transform.Translate(vec3(0, 0, -Time::GetDeltaTime()*player->movementSpeed));
				player->UpdateCollider();
			}
		}
	}
	if (Input::GetKey(GLFW_KEY_DOWN))
	{
		player->gameObject->transform.Translate(vec3(0, 0, -Time::GetDeltaTime()*player->movementSpeed));
		player->gameObject->transform.Rotate(vec3(-Time::GetDeltaTime() * 200, 0, 0));
		player->UpdateCollider();
		for (BoxCollider boxCollider : mazeCubesColliders)
		{
			HitInfo hit = player->IsColliding(boxCollider);
			if (hit.isColliding == true)
			{
				player->gameObject->transform.Translate(vec3(0, 0, +Time::GetDeltaTime()*player->movementSpeed));
				player->UpdateCollider();
			}
		}

	}
	if (Input::GetKey(GLFW_KEY_LEFT))
	{
		player->gameObject->transform.Translate(vec3(+Time::GetDeltaTime()*player->movementSpeed, 0, 0));
		player->gameObject->transform.Rotate(vec3(0, 0, +Time::GetDeltaTime() * 200));
		player->UpdateCollider();
		for (BoxCollider boxCollider : mazeCubesColliders)
		{
			HitInfo hit = player->IsColliding(boxCollider);
			if (hit.isColliding == true)
			{
				player->gameObject->transform.Translate(vec3(-Time::GetDeltaTime()*player->movementSpeed, 0, 0));
				player->UpdateCollider();
			}
		}
	}
	if (Input::GetKey(GLFW_KEY_RIGHT))
	{
		player->gameObject->transform.Translate(vec3(-Time::GetDeltaTime()*player->movementSpeed, 0, 0));
		player->gameObject->transform.Rotate(vec3(0, 0, -Time::GetDeltaTime() * 200));
		player->UpdateCollider();
		for (BoxCollider boxCollider : mazeCubesColliders)
		{
			HitInfo hit = player->IsColliding(boxCollider);
			if (hit.isColliding == true)
			{
				player->gameObject->transform.Translate(vec3(+Time::GetDeltaTime()*player->movementSpeed, 0, 0));
				player->UpdateCollider();
			}
		}
	}
	for (Obstacle obs : obstacles)
	{
		float distance = glm::distance(obs.gameObject->transform.position, player->gameObject->transform.position);
		if (distance < 20)
		{
			HitInfo hit = obs.IsColliding(player->GetBoxCollider());
			if (hit.isColliding == true)
			{
				player->gameObject->transform.position = mazeGenerator.GetStartPosition();
				player->UpdateCollider();
			}
		}
	}

}
