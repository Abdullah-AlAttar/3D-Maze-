#pragma once
#include "GameObject.h"
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Rendering\ModelMesh.h"
#include "..\\Rendering\Loader.h"
#include "..\\Physics\BoxCollider.h"
class SpeedPowerUp
{

public:
	SpeedPowerUp(TexturedModel texuredModel, Transform transform);
	HitInfo IsColliding(BoxCollider other);
	BoxCollider GetBoxCollider();
	GameObject *gameObject;
	void UpdateCollider();
	bool IsEnabled();
	void Disable();
	void Enable();
private:
	bool enabled;
	BoxCollider boxCollider;
};