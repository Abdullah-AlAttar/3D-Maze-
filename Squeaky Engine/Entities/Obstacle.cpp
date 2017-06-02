#include "Obstacle.h"

Obstacle::Obstacle(string modelPath, string texturePath, Transform transform,bool horizontal)
{
	loader = new Loader();
	RawModel rawModel = loader->LoadToVAO(ModelMesh::LoadModel(modelPath));
	Texture texture(loader->TextureFromFile(texturePath));
	TexturedModel texModel(rawModel, texture);
	gameObject = new GameObject(texModel, transform);

	gameObject->name = "obstacle";
	boxCollider = BoxCollider(gameObject->transform.position - vec3(0.8f,0.8f,0.8f),
		gameObject->transform.position + vec3(0.8f,0.8f,0.8f),
		this->gameObject->name);
	this->movementSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5))+0.9f;
	percent = 0;
	inc = true;
	this->horizontal = horizontal;
}

Obstacle::Obstacle(TexturedModel texuredModel, Transform transform, bool horizontal)
{
	gameObject = new GameObject(texuredModel, transform);
	gameObject->name = "obstacle";
	vec3 offset = vec3(0.75f, 0.75f, 0.75f);
	boxCollider = BoxCollider(gameObject->transform.position - offset,
		gameObject->transform.position + offset,
		this->gameObject->name);
	this->movementSpeed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5)) + 0.9f;
	percent = 0;
	inc = true;
	this->horizontal = horizontal;
}


HitInfo Obstacle::IsColliding(BoxCollider other)
{
	return boxCollider.OnCollision(other);
}

BoxCollider Obstacle::GetBoxCollider()
{
	return boxCollider;
}

void Obstacle::UpdateCollider()

{
		vec3 offset = vec3(0.75f, 0.75f, 0.75f);
	this->boxCollider.SetMinExtent(gameObject->transform.position - offset);
	this->boxCollider.SetMaxExtent(gameObject->transform.position + offset);
}

float Obstacle::GetMovementSpeed()
{
	return movementSpeed;
}

bool Obstacle::IsIncreasing()
{
	if (percent >= 1)
	{
		inc = false;
		return inc;
	}
	else if (percent <= 0)
	{
		inc = true;
		return inc;
	}
	else
		return inc;
}

bool Obstacle::IsHorizontal()
{
	return horizontal;
}
