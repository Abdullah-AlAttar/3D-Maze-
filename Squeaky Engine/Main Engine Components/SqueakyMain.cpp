
#include "WindowManager.h"
#include "..\Rendering\Loader.h"
#include "..\Models\RawModel.h"
#include "..\Rendering\Renderer.h"
#include "..\Shaders\Shader.h"
#include "..\Models\TexturedModel.h"
#include "..\Textures\Texture.h"
#include "..\Entities\GameObject.h"
#include "..\Input\Input.h"
#include "..\Rendering\Camera.h"
#include "..\Rendering\ModelMesh.h"
#include "..\Textures\Texture.h"
#include "Time.h"
#include "SceneManager.h"
#include <iostream>
#include <vector>
#include "..\\Maze\MazeGenerator.h"
#include "..\Entities\Player.h"
#include "..\Entities\Enemy.h"
#include <chrono>
#include <thread>
#include "..\Audio Manager\AudioManager.h"
#include <irrKlang.h>
#include "UtilityFunctions.h"
#include "..\Entities\Obstacle.h"
#include <glm\gtc\noise.hpp>
#include "Util.h"
#include "..\\Entities\SpeedPowerUp.h"
#include <filesystem>
#include <random>       // std::default_random_engine
#include <chrono>    

enum GameMode { EAZY, HARD };
enum CameraMode { FIXED_MODE, FPS_MODE, TP_MODE, FREE_MODE };
GameMode gameMode = GameMode::EAZY;
CameraMode camMode = CameraMode::FPS_MODE;


using namespace  Util;
int main()
{
	srand(time(0));

	unique_ptr<AudioManager> audioManager(new AudioManager());
	string audioPath = "Assests\\Audio\\" + to_string(rand() % 5) + ".ogg";
	//char * path = new char[audioPath.size()+1];
	char path[5][100] =
	{
		"Assests\\Audio\\0.ogg",
		"Assests\\Audio\\1.ogg",
		"Assests\\Audio\\2.ogg",
		"Assests\\Audio\\3.ogg",
		"Assests\\Audio\\4.ogg",
	};
	audioManager->Play2DSound(path[rand()%5], true);
	int seed = rand();

	MazeGenerator mazeGenerator(50, 20, seed, 35);
	mazeGenerator.GenerateMap();

	unique_ptr<WindowManager> window(new WindowManager());

	unique_ptr<Loader> meshLoader(new Loader());

	unique_ptr<Renderer> renderer(new Renderer());
	unique_ptr<SceneManager> sceneManager(new SceneManager());
	Shader shader;
	FixedCamera fixedCam(vec3(0, 1, -20));
	FPSCamera fpsCam(vec3(0, 1, 0));
	FreeCamera freeCam(vec3(0, 5, 0));

	window->Start();
	shader.LoadFromFile(GL_VERTEX_SHADER, "Shaders\\vs.glsl");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "Shaders\\fs.glsl");
	shader.CreateAndLinkProgram();
	RawModel moonRawModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\moon\\moon.obj"));
	RawModel planeRawModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\plane\\plane1.obj"));
	RawModel boxModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\Box\\box.obj"));
	RawModel obstacleModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\obstacle\\obstacle.obj"));
	RawModel capsuleModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\capsule\\capsule.obj"));
	RawModel skyeboxModel = meshLoader->LoadToVAO(ModelMesh::LoadModel("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\skybox\\skybox.obj"));
	//Texture texture(meshLoader->TextureFromFile("Assests\\Textures\\mario.png"));
	Texture moonTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\moon\\moontex.jpg"));
	Texture planeTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\plane\\black.jpg"));
	Texture boxTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\Box\\box.png"));
	Texture obstacleTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\moon\\moontex.jpg"));
	Texture capsuleTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\plane\\download.jpg"));
	Texture skyeboxTexture(meshLoader->TextureFromFile("C:\\Users\\runmd\\Source\\Repos\\testing_opengl\\testing_opengl\\Assests\\Models\\skybox\\skybox.png"));
	TexturedModel moonTexModel(moonRawModel, moonTexture);
	TexturedModel planeTexturedModel(planeRawModel, planeTexture);
	TexturedModel boxTexturedModel(boxModel, boxTexture);
	TexturedModel obstacleTexturedModel(obstacleModel, obstacleTexture);
	TexturedModel capsuleTexturedModel(capsuleModel, capsuleTexture);
	TexturedModel skyeboxTexturedModel(skyeboxModel, skyeboxTexture);
	GameObject box(boxTexturedModel, Transform(vec3(0, 1.0f, 0), vec3(0, 0, 0), vec3(1, 2, 1)));
	GameObject moon(moonTexModel, Transform(vec3(0, -5.0f, 0), vec3(0, 0, 0), vec3(1, 2, 1)));
	GameObject skybox(skyeboxTexturedModel, Transform(vec3(0, -5.0f, 0), vec3(0, 0, 0), vec3(4, 4, 4)));
	GameObject plane(planeTexturedModel, Transform(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));
	sceneManager->gameObjects.push_back(&plane);
	sceneManager->gameObjects.push_back(&skybox);
	unique_ptr<Player> player(new Player("Assests\\Models\\sun\\smoothSun.obj", "Assests\\Models\\sun\\suntex.jpg"));
	sceneManager->gameObjects.push_back(player->gameObject);
#pragma region Maze Boxes Generation
	int nameCnt = 0;
	List<GameObject *>mazeBoxes;
	List<BoxCollider> mazeBoxexColliders;
	List<pair<float, float>> scales;
	GameObject *tmpBox = NULL;
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
				//scales.push_back({ 1.0f, tmpBox->transform.scale.y });
				mazeBoxes.push_back(tmpBox);
				mazeBoxexColliders.push_back(BoxCollider(tmpBox->transform.position - VECTOR_ONE,
					tmpBox->transform.position + VECTOR_ONE,
					tmpBox->name));
			}
		}
	}

	player->gameObject->transform.position = mazeGenerator.GetStartPosition();
	/*tmpBox = new GameOjbect(boxTexturedModel, Transform(vec3(-mazeGenerator.GetWidth() / 2 + mazeGenerator.GetWidth() - 1.0f, 1, mazeGenerator.GetHeight() / 2), vec3(0, 0, 0), vec3(mazeGenerator.GetWidth(), 1, 1)));
	tmpBox->name = "box" + to_string(nameCnt++);
	mazeBoxes.push_back(tmpBox);
	mazeBoxexColliders.push_back(BoxCollider(tmpBox->transform.position - vec3(mazeGenerator.GetWidth(), 1, 1),
		tmpBox->transform.position + vec3(mazeGenerator.GetWidth(), 1, 1),
		tmpBox->name));
	tmpBox = new GameOjbect(boxTexturedModel, Transform(vec3(-mazeGenerator.GetWidth() / 2 + mazeGenerator.GetWidth() - 1.0f, 1, mazeGenerator.GetHeight() / 2 + (mazeGenerator.GetHeight() - 1) * 2), vec3(0, 0, 0), vec3(mazeGenerator.GetWidth(), 1, 1)));
	tmpBox->name = "box" + to_string(nameCnt++);
	mazeBoxes.push_back(tmpBox);
	mazeBoxexColliders.push_back(BoxCollider(tmpBox->transform.position - vec3(mazeGenerator.GetWidth(), 1, 1),
		tmpBox->transform.position + vec3(mazeGenerator.GetWidth(), 1, 1),
		tmpBox->name));*/
#pragma endregion
#pragma region Obstacle Generation region

	List<Obstacle> _obstacles;
	vector<Path> res = mazeGenerator.GetAvailablePathes();

	vector<PathInWorld> availablePathes;
	for (auto i : res)
		availablePathes.push_back(PathInWorld(
			UtilityFunctions::IndicesToWorldCoordiantes(i.start, mazeGenerator.GetWidth(), mazeGenerator.GetHeight()),
			UtilityFunctions::IndicesToWorldCoordiantes(i.end, mazeGenerator.GetWidth(), mazeGenerator.GetHeight())));
	for (int i = 0; i < availablePathes.size(); ++i)
	{
		_obstacles.push_back(Obstacle(obstacleTexturedModel,
			Transform(availablePathes[i].start, VECTOR_ZERO, vec3(0.9f, 0.9f, 0.9f)),
			(availablePathes[i].start.z - availablePathes[i].end.z) == 0));
	}

#pragma endregion
#pragma region  powerUps init
	GameObject capsule(capsuleTexturedModel, Transform(player->gameObject->transform.position, vec3(90, 0, -90), vec3(0.3f, 0.3f, 0.3f)));
	List<SpeedPowerUp> powerUps;
	List<vec3> emptyPositions = mazeGenerator.GetEmptyPositions();
	for (int i = 0; i < emptyPositions.size(); ++i)
	{
		if (rand() % 100 < 20)
		{
			powerUps.push_back(SpeedPowerUp(capsuleTexturedModel,
				Transform(emptyPositions[i], vec3(90, 0, -90), vec3(0.2f, 0.2f, 0.6f))));
		}
	}
#pragma endregion
#pragma region Shader And Lighting

	shader.AddUniforms
	({
		"MVP","viewMatrix","lightColor","lightPosition","modelMatrix","shineDamper","reflectivity",
		"viewPos","material.ambient","material.specular","material.diffuse","material.shininess",
		"defaultLight.ambient","defaultLight.diffuse" ,"defaultLight.specular",

		"pointLights[0].position","pointLights[0].ambient","pointLights[0].diffuse","pointLights[0].specular",
		"pointLights[0].constant","pointLights[0].linear","pointLights[0].quadratic",
		"pointLights[1].position","pointLights[1].ambient","pointLights[1].diffuse","pointLights[1].specular",
		"pointLights[1].constant","pointLights[1].linear","pointLights[1].quadratic",
		"pointLights[2].position","pointLights[2].ambient","pointLights[2].diffuse","pointLights[2].specular",
		"pointLights[2].constant","pointLights[2].linear","pointLights[2].quadratic",
		"pointLights[3].position","pointLights[3].ambient","pointLights[3].diffuse","pointLights[3].specular",
		"pointLights[3].constant","pointLights[3].linear","pointLights[3].quadratic",

		"dirLight.direction","dirLight.ambient","dirLight.diffuse","dirLight.specular",

		"spotLight.position","spotLight.direction","spotLight.ambient",	"spotLight.diffuse","spotLight.specular",
		"spotLight.constant","spotLight.linear","spotLight.quadratic","spotLight.cutOff","spotLight.outerCutOff",
		});
	shader.Use();
	shader.SetUniform(shader("material.ambient"), vec3(0.6f, 0.4f, 0.31f));
	shader.SetUniform(shader("material.diffuse"), vec3(1.0f, 0.4f, 0.4f));
	shader.SetUniform(shader("material.specular"), vec3(0.9f, 0.9f, 0.9f));
	shader.SetUniform(shader("material.shininess"), 64.0f);
	Light defaultLight(UtilityFunctions::IndicesToWorldCoordiantes(vec2(mazeGenerator.GetWidth() / 2, mazeGenerator.GetHeight() / 2), mazeGenerator.GetWidth(), mazeGenerator.GetHeight()) + VECTOR_UP * 52.0f
		, vec3(0.03f, 0.03f, 0.03f), vec3(0.2f, 0.2f, 0.2f), VECTOR_ONE);
	shader.SetUniform(shader("defaultLight.position"), defaultLight.position);
	shader.SetUniform(shader("defaultLight.ambient"), defaultLight.ambient);
	shader.SetUniform(shader("defaultLight.diffuse"), defaultLight.diffuse);
	shader.SetUniform(shader("defaultLight.specular"), defaultLight.specular);

	PointLight pl1(mazeGenerator.GetStartPosition() + vec3(0, 6, 0), vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1, 1, 1), 1.0f, 0.09f, 0.0032f);
	PointLight pl2(mazeGenerator.GetFinishPosition() + vec3(0, 6, 0), vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	PointLight pl3(vec3(50, 5, -50), vec3(0.5f, 0.05f, 0.05f), vec3(0.8f, 0.2f, 0.2f), vec3(1, 0.8, 0.8), 1.0f, 0.09f, 0.032f);
	PointLight pl4(vec3(-50, 5, 50), vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8, 0.8f), vec3(1, 1, 1), 1.0f, 0.09f, 0.032f);
	DirectionalLight dr(vec3(-0.2f, -1.0f, -0.3f), vec3(0.05f, 0.05f, 0.05f), vec3(0.5f, 0.5f, 0.5), vec3(0.5f, 0.5f, 0.2f));

	SpotLight sl(vec3(30, 10, 30), vec3(0, 0, -1), vec3(0.2f, 0.02f, 0.02f), VECTOR_ONE, VECTOR_ONE, 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.5f)));

	shader.SetPointLightUniforms(pl1, 0);
	shader.SetPointLightUniforms(pl2, 1);
	shader.SetPointLightUniforms(pl3, 2);
	shader.SetPointLightUniforms(pl4, 3);
	shader.SetDirectionalLightUniforms(dr);
	shader.SetSpotLightUniforms(sl);

#pragma endregion

	fixedCam.SetupProjectionMatrix(window->WIDTH / window->HEIGHT*1.0f, 0.1f, 900.0f);
	fpsCam.SetupProjectionMatrix(window->WIDTH / window->HEIGHT*1.0f, 0.1f, 900.0f);
	freeCam.SetupProjectionMatrix(window->WIDTH / window->HEIGHT*1.0f, 0.1f, 900.0f);
	Time::Start();
	vec3 deathPosition;
	vec3 startPosition = mazeGenerator.GetStartPosition() + vec3(-5, 0, 0);
	float returnToStartPositionPercent = 0;
	float timeToReturnToStartPosition = 2;
	float changingMapTime = 4;
	float changingMapPercent = 0;
	bool inc = true;
	float percent = 0;
	float drawCounter = 0;
	bool doDrawing = true, playedOnce = false;
	int speedBoost = 3;
	bool step1 = true;
	List<vec3> currentTransforms;
	List<vec3> targetTranforms;
	List<vec3> originalTransforms;

	vec3 centerPosition;
	for (int i = 0; i < mazeBoxes.size(); ++i)
	{
		centerPosition += mazeBoxes[i]->transform.position;
	}
	centerPosition /= mazeBoxes.size();
	while (window->IsRunning())
	{
		shader.Use();
		Time::Update();
		window->BeginUpdate();

		if (Input::GetKeyDown(GLFW_KEY_R))
		{
			currentTransforms.clear();
			targetTranforms.clear();
			player->playerState = PlayerState::CHANGING_MAP;
			changingMapPercent = 0;
			mazeGenerator.GenerateMap();
			for (int i = 0; i < mazeBoxes.size(); ++i)
			{
				
				originalTransforms.push_back(mazeBoxes[i]->transform.position);
			}
			int i = 0;
			double step = 2 * 3.14 / mazeBoxes.size();
			for (double angle = 0; angle < 2 * 3.14 && i < mazeBoxes.size(); angle += step)
			{
				auto r = mazeGenerator.GetHeight() + mazeGenerator.GetWidth();
				auto pX = centerPosition.x + r * glm::cos(angle);
				auto pY = centerPosition.z + r * glm::sin(angle);
				//currentTransforms.push_back(mazeBoxes[i]->transform.position + vec3(rand() % 25 - 12, 0, rand() % 25 - 12));
				currentTransforms.push_back(vec3(pX, mazeBoxes[i++]->transform.position.y, pY));
			}
			mazeBoxes.clear();
			mazeBoxexColliders.clear();
			originalTransforms.clear();
			for (int i = 0; i < mazeGenerator.GetWidth(); ++i)
			{
				for (int j = 0; j < mazeGenerator.GetHeight(); ++j)
				{
					if (mazeGenerator.mMaze[i][j] == 1)
					{
						targetTranforms.push_back(vec3(-mazeGenerator.GetWidth() / 2 + i * 2, 1, mazeGenerator.GetHeight() / 2 + j * 2));

						tmpBox = new GameObject(boxTexturedModel, Transform(targetTranforms[i],
							vec3(0, 0, 0),
							glm::vec3(1, 6 * (glm::perlin(glm::vec2((i + 1) / 16.0f, (j + 1) / 16.0f)) + 1), 1)));
						mazeBoxes.push_back(tmpBox);
						mazeBoxexColliders.push_back(BoxCollider(targetTranforms[targetTranforms.size() - 1] - VECTOR_ONE,
							targetTranforms[targetTranforms.size() - 1] + VECTOR_ONE,
							tmpBox->name));

					}
				}
			}

			player->gameObject->transform.position = mazeGenerator.GetStartPosition();
			res = mazeGenerator.GetAvailablePathes();
			_obstacles.clear();
			availablePathes.clear();
			for (auto i : res)
				availablePathes.push_back(PathInWorld(
					UtilityFunctions::IndicesToWorldCoordiantes(i.start, mazeGenerator.GetWidth(), mazeGenerator.GetHeight()),
					UtilityFunctions::IndicesToWorldCoordiantes(i.end, mazeGenerator.GetWidth(), mazeGenerator.GetHeight())));
			for (int i = 0; i < availablePathes.size(); ++i)
			{
				_obstacles.push_back(Obstacle(obstacleTexturedModel,
					Transform(availablePathes[i].start, VECTOR_ZERO, vec3(0.9f, 0.9f, 0.9f)),
					(availablePathes[i].start.z - availablePathes[i].end.z) == 0));
			}
			powerUps.clear();
			emptyPositions = mazeGenerator.GetEmptyPositions();
			for (int i = 0; i < emptyPositions.size(); ++i)
			{
				if (rand() % 100 < 20)
				{
					powerUps.push_back(SpeedPowerUp(capsuleTexturedModel,
						Transform(emptyPositions[i], vec3(90, 0, -90), vec3(0.2f, 0.2f, 0.6f))));
				}
			}
			std::shuffle(currentTransforms.begin(), currentTransforms.end(), std::default_random_engine(seed));
		
		}
		if (player->playerState == PlayerState::CHANGING_MAP)
		{
			int i;

			//	cout << mazeBoxes.size() << " " << currentTransforms.size() << " " << targetTranforms.size() << endl;
			if (step1)
			{
				for (i = 0; i < min(currentTransforms.size(), mazeBoxes.size()); ++i)
				{
					mazeBoxes[i]->transform.position = glm::mix(originalTransforms[i], currentTransforms[i], changingMapPercent / changingMapTime);
				}
				for (; i < mazeBoxes.size(); ++i)
				{
					mazeBoxes[i]->transform.position = targetTranforms[i];
				}
			}
			else
			{
				for (i = 0; i < min(currentTransforms.size(), mazeBoxes.size()); ++i)
				{
					mazeBoxes[i]->transform.position = glm::mix(currentTransforms[i], targetTranforms[i], changingMapPercent / changingMapTime);
				}
				for (; i < mazeBoxes.size(); ++i)
				{
					mazeBoxes[i]->transform.position = targetTranforms[i];
				}
			}
			if (changingMapPercent > changingMapTime)
				if (step1)
				{
					changingMapPercent = 0;
					step1 = false;
				}
				else
				{
					player->playerState = PlayerState::MOVING;
					step1 = true;
					for (i = 0; i < min(currentTransforms.size(), mazeBoxes.size()); ++i)
					{
						mazeBoxes[i]->transform.position = targetTranforms[i];
					}
				}
			cout << changingMapPercent << endl;
			changingMapPercent += Time::GetDeltaTime();
		}
#pragma region handling camera modes 
		if (Input::GetKeyDown(GLFW_KEY_1))
		{
			camMode = CameraMode::FIXED_MODE;
		}
		if (Input::GetKeyDown(GLFW_KEY_2))
		{
			camMode = CameraMode::FPS_MODE;
		}
		if (Input::GetKeyDown(GLFW_KEY_3))
		{
			camMode = CameraMode::FREE_MODE;
		}
		if (camMode == CameraMode::FIXED_MODE)
		{
			fixedCam.SetTarget(player->gameObject->transform);
			renderer->Prepare(fixedCam.GetViewMatrix(), fixedCam.GetProjectionMatrix());
			shader.SetUniform(shader("viewPos"), defaultLight.position);

			sl.position = VECTOR_UP * 100.0f;
			shader.SetUniform(shader("spotLight.position"), sl.position);
		}
		else if (camMode == CameraMode::FPS_MODE)
		{
			renderer->Prepare(fpsCam.GetViewMatrix(), fpsCam.GetProjectionMatrix());
			shader.SetUniform(shader("viewPos"), fpsCam.GetCameraPosition());
			//	player->gameObject->transform.position = fpsCam.GetCameraPosition();
			fpsCam.SetCameraPosition(player->gameObject->transform.position + VECTOR_UP * 1.5f);
			sl.position = fpsCam.GetCameraPosition();
			sl.direction = fpsCam.GetCameraFront();
			shader.SetUniform(shader("spotLight.position"), sl.position);
			shader.SetUniform(shader("spotLight.direction"), sl.direction);
		}
		else if (camMode == CameraMode::FREE_MODE)
		{
			renderer->Prepare(freeCam.GetViewMatrix(), freeCam.GetProjectionMatrix());
			sl.position = VECTOR_UP * 100.0f;
			shader.SetUniform(shader("spotLight.position"), sl.position);
			shader.SetUniform(shader("viewPos"), freeCam.GetCameraPosition());
		}

#pragma endregion

		//defaultLight.position = player->gameObject->transform.position + vec3(0, 3, 0);
		//shader.SetUniform(shader("defaultLight.position"), defaultLight.position);
		//	moonGameOjbect.transform.RotateAround(vec3(mazeGenerator.GetWidth(),0,mazeGenerator.GetHeight()), VECTOR_FORWARD,15, angle);
		pl3.position = player->gameObject->transform.position + vec3(0, 2, 0);
		shader.SetPointLightUniforms(pl3, 2);
		if (glm::sign((player->gameObject->transform.position.x - mazeGenerator.GetFinishPosition().x)) == 1)
		{
			deathPosition = player->gameObject->transform.position;
			player->playerState = PlayerState::DEAD;
			char path[] = "Assests\\audio\\nonono.ogg";
			audioManager->Play2DSound(path , false);
		}
		if (glm::distance(player->gameObject->transform.position, mazeGenerator.GetFinishPosition()) <= 0.5f)
		{
			player->playerState = PlayerState::WON;
		}
		if (player->playerState == PlayerState::MOVING)
		{
#pragma region player Movement in FPSMode
			if (camMode == CameraMode::FPS_MODE)
			{
				if (Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_LEFT))
				{
					//	fpsCam.ProcessKeyboard(LEFT, Time::GetDeltaTime()*camFlySpeed);
					vec3 playerFront = fpsCam.GetCameraFront();
					playerFront.y = 0;
					vec3 playerLeft = glm::cross(VECTOR_UP, playerFront);
					player->gameObject->transform.Translate(playerLeft*Time::GetDeltaTime()*player->movementSpeed);
					player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(-playerLeft * Time::GetDeltaTime()*player->movementSpeed);
							player->UpdateCollider();
						}
					}
				}
				if (Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_RIGHT))
				{
					//	fpsCam.ProcessKeyboard(RIGHT, Time::GetDeltaTime() * camFlySpeed);
						//fpsCam.ProcessKeyboard(LEFT, Time::GetDeltaTime()*camFlySpeed);
					vec3 playerFront = fpsCam.GetCameraFront();
					playerFront.y = 0;
					vec3 playerLeft = glm::cross(VECTOR_UP, playerFront);
					player->gameObject->transform.Translate(-playerLeft * Time::GetDeltaTime()*player->movementSpeed);
					player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(playerLeft*Time::GetDeltaTime()*player->movementSpeed);
							player->UpdateCollider();
						}
					}
				}
				if (Input::GetKey(GLFW_KEY_W) || Input::GetKey(GLFW_KEY_UP))
				{
					//fpsCam.ProcessKeyboard(FORWARD, Time::GetDeltaTime() * camFlySpeed);
					vec3 playerFront = fpsCam.GetCameraFront();
					playerFront.y = 0;
					player->gameObject->transform.Translate(playerFront*Time::GetDeltaTime()*player->movementSpeed);
					player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(-playerFront * Time::GetDeltaTime()*player->movementSpeed);
							player->UpdateCollider();
						}
					}
				}
				if (Input::GetKey(GLFW_KEY_S) || Input::GetKey(GLFW_KEY_DOWN))
				{
					//fpsCam.ProcessKeyboard(BACKWARD, Time::GetDeltaTime()*camFlySpeed);
					vec3 playerFront = fpsCam.GetCameraFront();
					playerFront.y = 0;
					player->gameObject->transform.Translate(-playerFront * Time::GetDeltaTime()*player->movementSpeed);
					player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(playerFront*Time::GetDeltaTime()*player->movementSpeed);
							player->UpdateCollider();
						}
					}
				}
				glm::vec2 smoothMouseInput = Input::FilterMouseMoves(Input::GetDeltaX(), Input::GetDeltaY());
				fpsCam.ProcessMouseMovement(smoothMouseInput.x, smoothMouseInput.y);
				fpsCam.ProcessMouseScroll(Input::GetOffsetY());
			}

#pragma endregion

#pragma region player Movement in fixedMode
			if (camMode == CameraMode::FIXED_MODE)
			{

				if (Input::GetKey(GLFW_KEY_W) || Input::GetKey(GLFW_KEY_UP))
				{
					player->gameObject->transform.Translate(vec3(0, 0, +Time::GetDeltaTime()*player->movementSpeed));
					player->gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(vec3(0, 0, -Time::GetDeltaTime()*player->movementSpeed));
							player->UpdateCollider();
						}
					}
				}
				if (Input::GetKey(GLFW_KEY_S) || Input::GetKey(GLFW_KEY_DOWN))
				{
					player->gameObject->transform.Translate(vec3(0, 0, -Time::GetDeltaTime()*player->movementSpeed));
					player->gameObject->transform.Rotate(vec3(-Time::GetDeltaTime() * player->turnSpeed, 0, 0));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(vec3(0, 0, +Time::GetDeltaTime()*player->movementSpeed));
							player->UpdateCollider();
						}
					}

				}
				if (Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_LEFT))
				{
					player->gameObject->transform.Translate(vec3(+Time::GetDeltaTime()*player->movementSpeed, 0, 0));
					player->gameObject->transform.Rotate(vec3(0, 0, +Time::GetDeltaTime() * player->turnSpeed));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(vec3(-Time::GetDeltaTime()*player->movementSpeed, 0, 0));
							player->UpdateCollider();
						}
					}
				}
				if (Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_RIGHT))
				{
					player->gameObject->transform.Translate(vec3(-Time::GetDeltaTime()*player->movementSpeed, 0, 0));
					player->gameObject->transform.Rotate(vec3(0, 0, -Time::GetDeltaTime() * player->turnSpeed));
					player->UpdateCollider();
					for (BoxCollider boxCollider : mazeBoxexColliders)
					{
						HitInfo hit = player->IsColliding(boxCollider);
						if (hit.isColliding == true)
						{
							player->gameObject->transform.Translate(vec3(+Time::GetDeltaTime()*player->movementSpeed, 0, 0));
							player->UpdateCollider();
						}
					}
				}
			}


#pragma endregion

#pragma region Handling Collision with Obstacles

			for (Obstacle obs : _obstacles)
			{

				float distance = glm::distance(obs.gameObject->transform.position, player->gameObject->transform.position);

				if (distance < 5)
				{
					HitInfo hit = obs.IsColliding(player->GetBoxCollider());
					if (hit.isColliding == true)
					{
						char path[] = "Assests\\audio\\boom1.ogg";
						audioManager->Play2DSound(path , false);
						deathPosition = player->gameObject->transform.position;
						//player->gameObject->transform.position = mazeGenerator.GetStartPosition() + vec3(-5, 0, 0);
						player->playerState = PlayerState::DEAD;
						/*cout << glm::distance(player->gameObject->transform.position, startPosition) << endl;
						timeToReturnToStartPosition = 25.0f / glm::distance(player->gameObject->transform.position, startPosition);
						cout << timeToReturnToStartPosition << endl;*/
						player->UpdateCollider();
					}
				}
			}
#pragma endregion

#pragma region handling collisions with powerUps

			for (int i = 0; i < powerUps.size(); ++i)
			{
				if (powerUps[i].IsEnabled())
				{
					float distance = glm::distance(powerUps[i].gameObject->transform.position, player->gameObject->transform.position);
					if (distance < 5)
					{
						HitInfo hit = powerUps[i].IsColliding(player->GetBoxCollider());
						if (hit.isColliding == true)
						{
							speedBoost++;
							cout << speedBoost << endl;
							powerUps[i].Disable();
						}
					}
				}
			}
#pragma endregion
#pragma region Cam Free Mode
			if (camMode == CameraMode::FREE_MODE)
			{

				if (Input::GetKey(GLFW_KEY_W))
				{
					freeCam.ProcessKeyboard(Camera_Movement::FORWARD, Time::GetDeltaTime() * 10);
				}
				if (Input::GetKey(GLFW_KEY_S))
				{
					freeCam.ProcessKeyboard(Camera_Movement::BACKWARD, Time::GetDeltaTime() * 10);

				}
				if (Input::GetKey(GLFW_KEY_A))
				{
					freeCam.ProcessKeyboard(Camera_Movement::LEFT, Time::GetDeltaTime() * 10);
				}
				if (Input::GetKey(GLFW_KEY_D))
				{
					freeCam.ProcessKeyboard(Camera_Movement::RIGHT, Time::GetDeltaTime() * 10);
				}
				if (Input::GetKey(GLFW_KEY_Q))
				{
					freeCam.ProcessKeyboard(Camera_Movement::UPWARD, Time::GetDeltaTime() * 10);
				}
				if (Input::GetKey(GLFW_KEY_E))
				{
					freeCam.ProcessKeyboard(Camera_Movement::DOWNWARD, Time::GetDeltaTime() * 10);
				}
				glm::vec2 smoothMouseInput = Input::FilterMouseMoves(Input::GetDeltaX(), Input::GetDeltaY());
				freeCam.ProcessMouseMovement(smoothMouseInput.x, smoothMouseInput.y);
				freeCam.ProcessMouseScroll(Input::GetOffsetY());
			}
#pragma endregion

			if (Input::GetKeyDown(GLFW_KEY_SPACE))
			{
				if (speedBoost > 0)
				{
					player->SetSpeed(12.0f);
				}
			}
			if (Input::GetKeyUp(GLFW_KEY_SPACE))
			{
				if (speedBoost > 0)
				{
					speedBoost--;
					player->ResetSpeed();
				}
			}

		}
#pragma region HandingPlayerDeath,Win
		if (player->playerState == PlayerState::DEAD)
		{
			player->gameObject->transform.position =
				glm::mix(deathPosition, startPosition, returnToStartPositionPercent / timeToReturnToStartPosition);
			returnToStartPositionPercent += Time::GetDeltaTime();
			fpsCam.SetCameraPosition(player->gameObject->transform.position);
			if (glm::distance(player->gameObject->transform.position, startPosition) < 2.0f)
			{
				player->playerState = PlayerState::MOVING;
				returnToStartPositionPercent = 0;
			}
		}
		if (player->playerState == PlayerState::WON)
		{
			if (playedOnce == false)
			{
				char path[] = "Assests\\Audio\\tada.wav";
				audioManager->Play2DSound(path, false);
				playedOnce = true;
			}
		}
#pragma endregion 

#pragma region Moving ,Drawing Obstacles


		drawCounter += Time::GetDeltaTime();
		if (drawCounter > 1.0f && doDrawing == false)
		{
			doDrawing = !doDrawing;
			drawCounter = 0;
		}
		if (drawCounter > 2.5f && doDrawing == true)
		{
			doDrawing = !doDrawing;
			drawCounter = 0;
		}
		for (int i = 0; i < _obstacles.size(); ++i)
		{
			bool isIncreasing = _obstacles[i].IsIncreasing();
			bool isHorizontal = _obstacles[i].IsHorizontal();
			_obstacles[i].percent += isIncreasing ? Time::GetDeltaTime() / _obstacles[i].GetMovementSpeed() : -Time::GetDeltaTime() / _obstacles[i].GetMovementSpeed();
			_obstacles[i].gameObject->transform.position = glm::mix(availablePathes[i].start, availablePathes[i].end, _obstacles[i].percent);
			if (isHorizontal)
			{
				_obstacles[i].gameObject->transform.Rotate(isIncreasing ? vec3(0, 0, -Time::GetDeltaTime() * 200 / _obstacles[i].GetMovementSpeed()) : vec3(0, 0, Time::GetDeltaTime() * 200 / _obstacles[i].GetMovementSpeed()));
			}
			else
			{
				_obstacles[i].gameObject->transform.Rotate(isIncreasing ? vec3(Time::GetDeltaTime() * 200 / _obstacles[i].GetMovementSpeed(), 0, 0) : vec3(-Time::GetDeltaTime() * 200 / _obstacles[i].GetMovementSpeed(), 0, 0));
			}
		}
		if (_obstacles.size())
		{
			renderer->BindGameObjectData(_obstacles[0].gameObject, shader);
			for (int i = 0; i < _obstacles.size(); ++i)
			{
				_obstacles[i].UpdateCollider();
				float distance = glm::distance(_obstacles[i].gameObject->transform.position, player->gameObject->transform.position);
				//vec3 scale = vec3(2 / distance, 2 / distance, 2 / distance);
				//_obstacles[i].gameObject->transform.scale = scale;
				if (gameMode == GameMode::EAZY)
				{
					if (distance < 30 || camMode == CameraMode::FREE_MODE)
						renderer->ApplyTransformationAndDraw(_obstacles[i].gameObject, shader);
				}
				else if (gameMode == GameMode::HARD)
				{
					if (doDrawing &&distance < 30 || camMode == CameraMode::FREE_MODE)
						renderer->ApplyTransformationAndDraw(_obstacles[i].gameObject, shader);
				}
			}
			renderer->UnbindCurrentGameObjectData(shader);
		}
#pragma endregion

#pragma region drawing boxes
		if (percent >= 1)
		{
			inc = false;
		}
		else if (percent <= 0)
		{
			inc = true;
		}
		percent += (inc) ? Time::GetDeltaTime() : -Time::GetDeltaTime();
		//for (int i = 0; i < mazeBoxes.size(); ++i)
		//{
		//	//	mazeBoxes[i]->transform.scale.y = glm::mix(scales[i].first, scales[i].second, percent);
		//	//  mazeBoxes[i]->transform.scale.y += sin(glfwGetTime()* ((float)(i+1) / (float)(mazeBoxes.size()+1))*5)*Time::GetDeltaTime()*2;
		//	//mazeBoxes[i]->transform.scale.y += glm::sin(Time::GetTime()*(i+1)/(mazeBoxes.size()))*Time::GetDeltaTime()*3;
		//	//	mazeBoxes[i]->transform.scale.y = glm::clamp(mazeBoxes[i]->transform.scale.y, 0.7f, 20.0f);
		//}
		renderer->BindGameObjectData(tmpBox, shader);
		for (auto box : mazeBoxes)
		{
			float distance = glm::distance(box->transform.position, player->gameObject->transform.position);
			if (distance < 30 || camMode == CameraMode::FREE_MODE)
				renderer->ApplyTransformationAndDraw(box, shader);
		}
		renderer->UnbindCurrentGameObjectData(shader);
#pragma endregion
#pragma region drawing powerUps
		if (powerUps.size())
		{
			renderer->BindGameObjectData(powerUps[0].gameObject, shader);
			for (int i = 0; i < powerUps.size(); ++i)
			{
				if (powerUps[i].IsEnabled())
				{
					float distance = glm::distance(powerUps[i].gameObject->transform.position, player->gameObject->transform.position);
					//powerUps[i].gameObject->transform.position.y = 0.3f*glm::sin(Time::GetTime())+1.5f;
					powerUps[i].gameObject->transform.Rotate(vec3(Time::GetDeltaTime() * 200, 0, 0));
					if (distance < 20 || camMode == CameraMode::FREE_MODE)
						renderer->ApplyTransformationAndDraw(powerUps[i].gameObject, shader);
				}
			}
			renderer->UnbindCurrentGameObjectData(shader);
		}
#pragma endregion
		renderer->Render(sceneManager->gameObjects, shader);
		shader.UnUse();
		window->EndUpdate();
	}

	return 0;
}
