#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "..\Models\RawModel.h"
#include "..\Models\TexturedModel.h"
#include "..\Entities\GameObject.h"
#include "..\Shaders\Shader.h"
#include <vector>
#include <iostream>
#include <map>

class Renderer
{
public:
	Renderer();
	void Prepare(mat4 viewMatrix,mat4 projectionMatrix);
	void Render(RawModel rawModel);
	void Render(TexturedModel texturedModel);
	void Render(GameObject &gameObject,Shader &shader);
	void Render(List<GameObject*> gameObjects, Shader &shader);
	void Render(map<int, List<GameObject*>> gameObjects, Shader &shader);
	void BindGameObjectData(GameObject *gameObject, Shader & shader);
	void UnbindCurrentGameObjectData(Shader &shader);
	void ApplyTransformationAndDraw(GameObject *gameObject, Shader &shader);
	void End();
private:
	mat4 viewMatrix;
	mat4 projectionMatrix;
};
