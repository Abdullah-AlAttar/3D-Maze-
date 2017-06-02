#pragma once
#include "GameObject.h"
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Rendering\ModelMesh.h"
#include "..\\Rendering\Loader.h"
#include "..\\Physics\BoxCollider.h"
enum PlayerState{MOVING,DEAD,COLLIDING,WON,CHANGING_MAP};
class Player
{
public:

	Player(string modelPath, string texturePath);
	GameObject *gameObject;
	float movementSpeed;
	float originalSpeed;
	float turnSpeed;
	void UpdateCollider();
	HitInfo IsColliding(BoxCollider other);
	BoxCollider &GetBoxCollider();
	vec3 GetOriginalPosition();
	PlayerState playerState;
	void SetSpeed(float speed);
	void ResetSpeed();
private:
	BoxCollider boxCollider;
	Loader *loader;
};
