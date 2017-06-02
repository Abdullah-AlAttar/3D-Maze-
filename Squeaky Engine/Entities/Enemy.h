#pragma once

#include "GameObject.h"
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Rendering\ModelMesh.h"
#include "..\\Rendering\Loader.h"
#include "..\\Physics\BoxCollider.h"

class Enemy
{
public:
	Enemy(string modelPath, string texturePath);
	GameObject *gameObject;
	float movementSpeed;
	HitInfo IsColliding(BoxCollider other);
	BoxCollider GetBoxCollider();
private:
	BoxCollider boxCollider;
	Loader *loader;
};

