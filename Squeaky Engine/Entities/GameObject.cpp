#include "GameObject.h"

TexturedModel GameObject::GetMesh()
{
	return this->mesh;
}
GameObject::GameObject(TexturedModel texturedModel, Transform transform)
{
	this->mesh = texturedModel;
	this->transform = transform;
	name = "no";
}
