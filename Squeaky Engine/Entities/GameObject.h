#pragma once
#include "..\Models\TexturedModel.h"
#include <glm/glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>
#include "..\\Main Engine Components\Util.h"
using namespace Util;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
#include <string>
using namespace std;

struct Transform
{

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	void Translate(vec3 amount) { position += amount; }
	void Rotate(vec3 amount) { rotation += amount; }
	void Scale(vec3 amount) { scale = amount; }
	void RotateAround(vec3 point, vec3 axis, float radius, float angle)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		float x = point.x + radius * cosAngle * sinAngle;
		float y = point.y + radius * sinAngle* sinAngle;
		float z = point.z + radius * cosAngle;
		if (axis == VECTOR_UP)
			this->position = (vec3(x, this->position.y + point.y, z));
		else if (axis == VECTOR_FORWARD)
			this->position = (vec3(x, y, point.z));
		else if (axis == VECTOR_RIGHT)
			this->position = (vec3(point.x, x, z));
	}
	Transform() :position(VECTOR_ZERO), rotation(VECTOR_ZERO), scale(VECTOR_ONE) {}
	Transform(vec3 _position, vec3 _rotation, vec3 _scale) :
		position(_position), rotation(_rotation), scale(_scale) {}
};

class GameObject
{
public:
	string name;
	TexturedModel GetMesh();
	Transform transform;
	GameObject(TexturedModel texturedModel, Transform transform);
private:
	TexturedModel mesh;
};