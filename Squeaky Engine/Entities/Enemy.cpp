#include "Enemy.h"

#include "Player.h"

Enemy::Enemy(string modelPath, string texturePath)
{
	loader = new Loader();
	RawModel rawModel = loader->LoadToVAO(ModelMesh::LoadModel(modelPath));
	Texture texture(loader->TextureFromFile(texturePath));
	TexturedModel texModel(rawModel, texture);
	gameObject = new GameObject(texModel, Transform(vec3(3, 1, 0), VECTOR_ZERO, VECTOR_ONE));
	gameObject->name = "Enemy";
	boxCollider = BoxCollider(gameObject->transform.position - VECTOR_ONE,
							  gameObject->transform.position + VECTOR_ONE,
							   this->gameObject->name);
}

HitInfo Enemy::IsColliding(BoxCollider other)
{
	return boxCollider.OnCollision(other);
}

BoxCollider Enemy::GetBoxCollider()
{
	return boxCollider;
}
