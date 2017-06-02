#pragma once
#include "GameObject.h"
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Rendering\ModelMesh.h"
#include "..\\Rendering\Loader.h"
#include "..\\Physics\BoxCollider.h"
class Obstacle
{

public:
	Obstacle(string modelPath, string texturePath,Transform transform,bool horizontal);
	Obstacle(TexturedModel texuredModel, Transform transform, bool horizontal);
	HitInfo IsColliding(BoxCollider other);
	BoxCollider GetBoxCollider();
	GameObject *gameObject;
	void UpdateCollider();
	float GetMovementSpeed();
	float percent;
	bool IsIncreasing();
	bool IsHorizontal();
private:
	bool inc;
	bool horizontal;
	float movementSpeed;
	BoxCollider boxCollider;
	Loader *loader;
};
