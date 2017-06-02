#include "Player.h"

Player::Player(string modelPath, string texturePath)
{
	originalSpeed=movementSpeed = 6.0f;
	turnSpeed = 200;
	loader = new Loader();
	RawModel rawModel = loader->LoadToVAO(ModelMesh::LoadModel(modelPath));
	Texture texture(loader->TextureFromFile(texturePath));
	TexturedModel texModel(rawModel, texture);
	gameObject = new GameObject(texModel, Transform(vec3(-20,1,5), VECTOR_ZERO, vec3(0.5f,0.5f,0.5f)));
	gameObject->name = "Player";
	boxCollider = BoxCollider(gameObject->transform.position - vec3(0.5f,0.5f,0.5f),
							  gameObject->transform.position + vec3(0.5f, 0.5f, 0.5f),
							  this->gameObject->name);
	playerState = PlayerState::MOVING;
}

void Player::UpdateCollider()
{
	this->boxCollider.SetMinExtent(gameObject->transform.position - vec3(0.5f, 0.5f, 0.5f));
	this->boxCollider.SetMaxExtent(gameObject->transform.position + vec3(0.5f, 0.5f, 0.5f));

}

HitInfo Player::IsColliding(BoxCollider other)
{
	return boxCollider.OnCollision(other);
}

BoxCollider& Player::GetBoxCollider()
{
	return boxCollider;
}

vec3 Player::GetOriginalPosition()
{
	return vec3(-20, 1, 5);
}

void Player::SetSpeed(float speed)
{
	movementSpeed = speed;
}

void Player::ResetSpeed()
{
	movementSpeed = originalSpeed;
}
