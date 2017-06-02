#pragma once
#include "GameObject.h"
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Rendering\ModelMesh.h"
#include "..\\Rendering\Loader.h"
#include "..\\Physics\BoxCollider.h"

class MazeCube
{
	MazeCube() {}

	MazeCube(string modelPath, string texturePath, Transform transform);

	GameObject *gameObject;
	HitInfo IsColliding(BoxCollider other);
	BoxCollider &GetBoxCollider();
private:
	BoxCollider boxCollider;
	Loader *loader;
};